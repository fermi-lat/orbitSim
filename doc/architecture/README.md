---
package: orbitSim
summary: gtorbsim and its static library - simulates the Fermi orbit (from a TLE via SGP4, or from tabulated ephemerides) and the attitude (survey, rocking profile, pointed, slews, Earth avoidance, SAA) and writes a simulated FT2 file.
layer: analysis
depends_on: [st_app, st_stream, tip, facilities]
used_by: []
last_reviewed_commit: 9c795b4
---

# orbitSim Architecture

**orbitSim** is the **`gtorbsim`** tool. It produces a *simulated*
spacecraft (FT2) file for a planned or hypothetical observation:

1. It takes an **orbit**, either propagated with SGP4 from a two-line
   element set or read from a tabulated ephemeris.
2. It **models the attitude** on a fixed time grid from a timeline: sky
   survey with a fixed rocking angle, a rocking *profile*, inertial
   pointings, and slews between them.
3. It applies **Earth avoidance**, **Earth-limb tracing** and **SAA**
   constraints.
4. It writes the result as an FT2 `SC_DATA` table from the template
   `data/ft2.tpl`.

Everything is in a static library (`orbitSim`) that only `gtorbsim`
links. The code came from the FSSC's TAKO-based planning tools (Giuseppe
Romeo, 2005–2008; later John Vernaleo and Joe Asercion). It is C-style C++
with `malloc`'d structs and `FILE*` I/O. The Doxygen mainpage
(`src/mainpage.h`) contains only an author line. User documentation is
`src/fermitools-fhelp/fhelp_files/gtorbsim.txt`.

## Purpose and scope

**Does:**
- Generate spacecraft positions on a regular grid (`Resolution` minutes)
  from:
  - a TLE (`tlederive`: in-house SGP4),
  - an FDF-style ephemeris (`yyyy_eph`), or
  - an STK ephemeris (`xyzll_eph`).
- Compute the LAT z/x-axis pointing for each grid point from:
  - a **TAKO** timeline (mission-planning ATS file),
  - an **ASFLOWN** timeline (as-flown attitude-command history), or
  - a **SINGLE** command: `SURVEY` (fixed rocking angle), `PROFILED`
    (17-point rocking profile) or `POINTED` (inertial RA/Dec).
- Estimate slew durations from a table-based model (`GLAST_slew_estimate`)
  or a fixed slew rate. Apply Earth occultation avoidance (`EAA`), limb
  tracing, and SAA entry from a user polygon.
- Write an FT2 file. It has `START`/`STOP`, `SC_POSITION`, geodetic
  position, zenith, SC z/x axes, orbit pole, `ROCK_ANGLE`, the quaternion
  `QSJ_1..4`, `IN_SAA` and `LIVETIME`.

**Does not:**
- Simulate photons or instrument response. `observationSim` (`gtobssim`)
  does that, and it can read the FT2 file this tool writes.
- Compute geomagnetic quantities. `B_MCILWAIN`, `L_MCILWAIN` and
  `GEOMAG_LAT` are written as `"NaN"`. `astro` has the IGRF model, but
  orbitSim does not use it.
- Use `astro`'s orbit, ephemeris or time code. It has its own copies of
  ASCA `atFunctions` (sidereal time, Sun position, rotations), its own
  SGP4, and its own MJD↔MET arithmetic.
- Ship any orbit data. The TLE, ephemeris, timeline and SAA polygon all
  come from the user.

## Position in the system

| Direction | Package | Why |
|-----------|---------|-----|
| Depends on | `st_stream` | `StreamFormatter` output throughout the library (`osf`, `losf`, `fosf` file-scope formatters). This is the **only** Fermi dependency of the library sources. |
| Depends on | `st_app` | `gtorbsim` (`src/orbSim/main.cxx`) is an `StApp`. |
| Depends on | `tip` | `main.cxx` creates the output from the template and fills `SC_DATA`. |
| Depends on | `facilities` | `main.cxx`: `commonUtilities::getDataPath("orbitSim")` to find `ft2.tpl`, and `Util::basename` for `FILENAME`. Not linked directly; it arrives through `tip`/`st_app`. |
| Link only | `st_facilities` | In `target_link_libraries`, but nothing in orbitSim includes or calls it. |
| Used by | — | Nothing includes `orbitSim/` headers. Its output files are read by `observationSim`, `Likelihood` (`gtltcube`), `gtbary`, and so on, like any FT2 file. |

The CMake file links `st_app st_facilities st_stream tip` `PUBLIC` into
the **library**. The library sources need only `st_stream`; the other
three are used by `main.cxx`. Since the library has no other users, this
only matters for the tier calculation. dependency-layers.md lists the
links, including the unused `st_facilities`.

## Key abstractions

There are no classes. The model is a set of C structs (`orbitSimStruct.h`)
and free functions:

| Struct | Role |
|--------|------|
| `InitI` | All run parameters: `start_MJD`/`stop_MJD`, timeline type and name (or the `SINGLE` command string), ephemeris file and function, `Units` (to km), `Resolution` (minutes, later converted to days), initial RA/Dec, output file, optional ASCII dump (`OptFile`), `saafile`, `EAA`, `ELT_OFF_START`/`STOP`, `occflag`. |
| `EphemData` | Spacecraft position on the time grid: `MJD[]`, `X/Y/Z[]` (km, equatorial inertial), `Lat/Long/Alt[]`, plus `ent`. |
| `Attitude` | Per grid point: `mjd`, position, `SatRA/SatDEC` (zenith), `Xra/Xdec`, `Yra/Ydec`, `Zra/Zdec`, `rockAngle`, `in_saa`, plus `ent`. Allocated with `allocateAttitude(n)`, grown and shrunk with `reallocateAttitude`. |
| `SurProf` | Rocking profile: `epoch` (ROCKSTART), `defofst`, 17 offsets (deg) and 17 times (s). |
| `atElemTle`, `AtTime`, `AtVect`, `AtRotMat`, `AtPolarVect`, `AtEulerAng` | TLE elements and the ASCA attitude-library vector types. |

| Source | Role |
|--------|------|
| `src/orbSim/main.cxx` | `orbitSimApp::run`: parameters, start/stop rounding, reading or propagating the ephemeris, dispatching on timeline type, writing FT2. |
| `src/OrbSim.cxx` | Timeline drivers: `parseInit` (init-file reader), `makeAttTako`, `makeAttAsFl`, `doCmd` (`SINGLE`), plus `parseProfile`, `parseInitParams`, `parseAsFline`, `getNPole`. |
| `src/read_ephem.cxx` | Ephemeris readers `yyyy_eph`, `xyzll_eph`, `tlederive`. Attitude builders: `MakeAtt`/`MakeAtt2` (one timeline segment), `MakeSurvey`, `MakeProfiled`/`doProfiled`/`InterpProfile`, `MakePointed`, `DoSlew`. Constraints: `occult`, `doLimbTrace`, `saa`. |
| `src/functUtil.cxx` | MJD/calendar/MET conversions, string matching (`match_str`, a tiny `^`/`$` pattern matcher replacing regex), timeline-line checks, `GetPos`/`GetPointedPos` (attitude for a survey offset or a pointing), `readTLE` + `sgp4`, `GetQuat`, SAA point-in-polygon, slew-time helpers. |
| `src/atFunctions.cxx` | A C++ copy of part of the ASCA `atFunctions` library (`atSun`, `atSidereal`, `atEarthOccult`, rotation matrices, geodetic conversion). |
| `src/GLAST_slew_estimate.cxx` | Dean Tsai's table-based slew-duration model, translated from MATLAB. About 300 of its 956 lines are three 91×91 lookup tables (`total_time`, `theta_y_mesh`, `theta_xz_mesh`). It has a `main()` under `#ifdef TEST` that is not built. |

Size: about 12,100 lines, all compiled except the `#ifdef TEST` block.
About 1,900 of them are headers, mostly Doxygen comments. There is no
generated code.

## Data flow and control flow

```
orbitSimApp::run
  read hidden pars: chatter, debug, EAA, ELT_OFF_START/STOP
  prompt typeinput
    FILE    → prompt initFile → parseInit(file)      // key = value lines; 12 required keys
    CONSOLE → prompt start_MJD … saafile one by one   // EphemFunc / TLType checked
  pars.Save()
  round start_MJD down to a Resolution boundary (stop rounding never applies; see gotchas)
  fopen(EphemName); Resolution /= 1440
  ephemeris = yyyy_eph | xyzll_eph | tlederive (StartTime, EndTime, Units, Resolution)
  Oat = makeAttTako | makeAttAsFl | doCmd (initf, ephemeris)
      build attitude on the grid from the timeline
      if occflag: occult(EAA, ELT_OFF_*) ; doLimbTrace
      saa(saafile)
      (TAKO/ASFLOWN) copy rows inside the original [start, stop] into a new Attitude
  tip createFile(OutPutFile, $FERMI_INST_DIR/refdata/fermi/orbitSim/ft2.tpl)
  primary + SC_DATA headers: DATE, DATE-OBS, DATE-END, TSTART, TSTOP, FILENAME
  for each row k:
      STOP = round((mjd[k] − 51910) × 86400); START = STOP − Resolution
      SC_POSITION (m), LAT/LON/RAD_GEO, RA/DEC_ZENITH, RA/DEC_SCZ, RA/DEC_SCX,
      RA/DEC_NPOLE (from ±2-row position difference), ROCK_ANGLE = −rockAngle,
      LAT_MODE = 5, LAT_CONFIG = 1, DATA_QUAL = 1,
      IN_SAA → LIVETIME = 0, else LIVETIME = 0.93 × Resolution,
      QSJ_1..4 from X/Y/Z axes (GetQuat)
```

### Inputs

| Input | Format (from the parser code) |
|-------|-------------------------------|
| Init file (`typeinput=file`) | `key = value` lines; `#` comments. Keys: `start_MJD`, `stop_MJD`, `TLType`, `Timeline`, `EphemName`, `EphemFunc`, `Units`, `Resolution`, `Initial_RA`, `Initial_DEC`, `OutPutFile`, `saafile` (all 12 required), plus optional `EAA`, `Earth_Avoid`, `OptFile`, `Chatter`, `Debug`. |
| TLE (`EphemFunc=tlederive`) | NORAD 3-line format. The name line must **start with `GLAST` or `FGRST`**. Only the first matching set is used. |
| FDF ephemeris (`yyyy_eph`) | `yyyy mm dd hh mm ss X Y Z Lat Lon`, with X/Y/Z scaled by `Units` to km. Lat/Lon are recomputed, not read. |
| STK ephemeris (`xyzll_eph`) | One comment line, then time, X/Y/Z, Lat, Lon. Lat/Lon are used as given. |
| TAKO timeline | The mission-planning ATS file. The first `Begin` line and the last line give the timeline span (`yyyy/ddd:hh:mm:ss`). Entries are `Survey`/`Obs`/`Profile`/`Slew` blocks with `RA`, `dec`, `offset`, `ROCKANGLE`, `ROCKSTART`, `ROCKDEFAULT`, `ROCKTIME`, `SS_Param` lines. |
| ASFLOWN timeline | `|`-separated lines: `yyyy-ddd-hh:mm:ss | ZenithPoint|InertialPoint|AutoRepoint | … | val1 | val2`. Profile lines use `SS_PARAM`/`ANG`/`TIM`/`RockDefault`/`RockStart`. |
| SINGLE command (`TimelnCmd`) | `|SURVEY| offset [| orbit_seconds] |` (default orbit 5722 s); `|PROFILED| epoch | 17×(time offset) … |`; `|POINTED| ra | dec |`. |
| SAA file | Either `SAAPOLYLAT = …` / `SAAPOLYLONG = …` lines, or plain `lon lat` pairs. `#` comments. At most 50 vertices. The literal file name `NULL` disables the SAA check. |

## Public interfaces

### Applications

| Tool | Par file | Notes |
|------|----------|-------|
| `gtorbsim` | `pfiles/gtorbsim.par` | Queried: `typeinput` (`file`\|`console`), `initFile`, or `start_MJD`, `stop_MJD`, `TLType` (`TAKO`\|`ASFLOWN`\|`SINGLE`), `Timeline` or `TimelnCmd`, `EphemName`, `EphemFunc` (`yyyy_eph`\|`xyzll_eph`\|`tlederive`), `Units`, `Resolution` (min), `Initial_RA`, `Initial_DEC`, `OutPutFile`, `saafile`. Hidden: `EAA` (default 5°), `ELT_OFF_START`/`ELT_OFF_STOP` (MJD), standard `chatter`/`clobber`/`debug`/`gui`/`mode`. Parameters are prompted one by one in `run()`, so with `typeinput=file` the console parameters are never prompted. Keyword matching is case-insensitive (`match_str` upper-cases). |

### C++ library

All headers in `orbitSim/` are installed (`OrbSim.h`, `read_ephem.h`,
`functions.h`, `atFunctions.h`, `orbitSimStruct.h`, `DynamicStruct.h`,
`GLAST_slew_estimate.h`). Nothing outside the package includes them, and
they are not designed as an API. Treat them as private.

### Python

None. `gtorbsim` is not wrapped by `sane`'s `gt_apps`.

### Environment variables

No `getenv` calls. `getDataPath("orbitSim")` resolves through
`FERMI_INST_DIR` (see `src/facilities/doc/architecture/` and "Runtime
environment" in ScienceTools' `docs/architecture/README.md`).

### File formats

- **Output:** FT2 created from `data/ft2.tpl`: an empty primary plus
  `SC_DATA` with 27 columns. `TELESCOP='GLAST'`, `TIMESYS='TT'`,
  `TIMEREF='LOCAL'`, `MJDREFI=51910`, `MJDREFF=7.4287e-4` (64.184 s).
  `SC_POSITION` is `3E` (float32, metres). `START`/`STOP` are written from
  integer seconds. The `CREATOR` keyword is left as the template's
  `'gtorbsim $Name:  $'`.
- **Side files written to the current directory:** `TakoAttitude.out`
  (TAKO mode) and `ASFLAttitude.out` (ASFLOWN mode). These are
  regression-test dumps that are always written. If `OptFile` is set in
  the init file, there is also an ASCII attitude dump.

## Build and test

- **CMake targets:** `orbitSim` (**STATIC**), `gtorbsim`. **No test
  target is built.**
- **Install:**
  - headers go to `include/orbitSim`
  - `pfiles/` goes to `syspfiles`
  - `data/ft2.tpl` goes to `${FERMI_INSTALL_REFDATADIR}/orbitSim`, which
    is where `getDataPath("orbitSim")` looks
- **Tests:** `src/test/main.cxx` is a line-by-line comparison of
  `TakoAttitude.out`/`ASFLAttitude.out` against reference files
  `TakoAttRef`/`ASFLAttRef`. It is **not in `CMakeLists.txt`** (nor in the
  old `SConscript`), and the reference files are not in the repository.
  In effect the package has no automated tests: ScienceTools'
  `recipe/tests/ST-unit-test` never lists orbitSim (neither list), and
  `fermitools-data` has no `outrefFiles/orbitSim` (see "Testing" in
  ScienceTools' `docs/architecture/README.md`). Exercise changes by
  running `gtorbsim` with a `SINGLE`/`SURVEY` command and a TLE, and
  inspecting the FT2 file (for example with `gtltcube` or `fv`).
- **Legacy:** `SConscript`, `orbitSimLib.py`, `.cvsignore`,
  `azure-pipelines.yml`.

## Invariants and gotchas

**Time handling** (from reading the code; not checked at runtime)
- **MET ignores leap seconds and the 64.184 s offset.** `do_mjd2met` is
  `(mjd − 51910) × 86400` (`functUtil.cxx`, `MJDREF` in `functions.h`).
  The template, however, declares TT with `MJDREFF` = 64.184 s, which
  makes MET 0 = 2001-01-01T00:00:00 **UTC**. If the input MJDs are UTC
  (TLE epochs and timeline times are), the output MET is short by the
  leap seconds since 2001: **5 s for dates after 2017-01-01**. At orbital
  speed, that is roughly 35–40 km along-track at a given MET. No
  leap-second table is used, so nothing expires, but nothing tracks new
  leap seconds either.
- **Rows are timestamped at the end of their interval.** Each row's
  position and attitude are computed at `mjd[k]`, which becomes `STOP`.
  `START = STOP − Resolution`. The FT2 convention (and the template's own
  column comments, "at start of interval") puts them at `START`.
- `START`/`STOP` come from `long` seconds, so a `Resolution` that is not
  a whole number of seconds is truncated. The interval-length warnings in
  the write loop fire then.
- The start time is rounded down to a `Resolution` boundary. The
  matching stop-time rounding in `main.cxx` can never apply: its condition
  `(stop − enmjd) − Resolution > 1e-6` is always false, because
  `enmjd ≥ stop`.
- `DATE-OBS`/`DATE-END` come from the rounded MJDs through `do_mjd2cal`
  and are labelled UTC with no leap-second handling.

**Expiring inputs**
- TLE epoch years are two-digit, with the pivot at **50**: `YY > 50` is
  read as 19YY, so TLEs from **2051** on will be misread. Fermi TLEs
  should use 57 by NORAD convention, which makes no difference until then.
- If the TLE epoch is after `start_MJD`, that is an error ("TLE file is
  too new"). If the run ends more than 30 days after the epoch, that is
  only a **warning**, and SGP4 accuracy degrades quickly with TLE age.
- The ephemeris files and TAKO/ASFLOWN timelines come from the user and
  have no expiry in the code. The TAKO run is clipped to the timeline's
  span: if `stop_MJD` is past the end, it is moved to the end with a
  warning.

**Output content**
- `LIVETIME` is hard-coded to **93 %** of the interval outside the SAA,
  and 0 inside. `LAT_MODE=5`, `LAT_CONFIG=1` and `DATA_QUAL=1` are
  constants. The template comment on `LAT_CONFIG` says "0=nominal", which
  is the old convention.
- `B_MCILWAIN`, `L_MCILWAIN` and `GEOMAG_LAT` are set from the **string**
  `"NaN"`, so tip has to convert it for a float column. Any analysis that
  cuts on these columns gets nothing useful.
- `SC_POSITION` is float32, so it has about 0.5 m resolution at orbital
  radius. That is fine for timing, but not for precise geometry.
- `TELESCOP` is `GLAST`. Tools that check it, such as `gtbary`, accept
  both `GLAST` and `FERMI`.
- `RA_NPOLE`/`DEC_NPOLE` come from a ±2-row position difference. With
  fewer than three rows the boundary branches read past the end of the
  arrays (from reading the code).

**Robustness** (from reading the code; not checked at runtime)
- `makeAttTako` copies rows into the result only where
  `OAtt->mjd[i] == ephem->MJD[i]`, an **exact floating-point comparison**.
  Rows that differ by rounding are dropped silently, and the result is
  shrunk to the rows kept. Its guard `if (k > oinum)` should be `>=`.
- In `saa()`, the `SAAPOLYLAT`/`SAAPOLYLONG` format stores vertices from
  index 1, but the plain `lon lat` format stores them from index 0. The
  polygon-closure check and the later code assume index 1, so with the
  plain format the first vertex is skipped and an uninitialized element
  is read. A missing SAA file is reported with `osf.err()` and the run
  continues **without any SAA**.
- `parseInit` removes the last character of every line, assuming it is
  `\n`. A final line without a newline loses its last character, which
  can corrupt a file name or a number. Any of the 12 required keys that is
  missing gives the generic "Something is wrong in the init file" error.
- `makeAttTako` reopens the timeline with `fopen` without closing the
  first handle, and never frees the working `Attitude`. When the timeline
  extends the range, it frees and re-reads `ephem`, which leaves the
  caller's `ephemeris` pointer dangling. `main` does not use it after
  that.
- `match_str` upper-cases both arguments. Without `^…$` anchors it does
  a **substring** match, so `typeinput`, `TLType`, `EphemFunc` and the
  `SINGLE` keywords in `main.cxx`/`doCmd` match anything that contains
  the keyword. For anchored patterns it allocates `len−1` bytes and then
  writes `len−1` characters plus the terminator. It also calls
  `sprintf(STR, "%s%c", STR, c)` on overlapping buffers. Both are
  undefined behaviour that happens to work.
- `SINGLE` parsing runs `strtok` on `std::string::c_str()` cast to
  `char*`.
- `GLAST_slew_estimate.cxx` does `#define end 90`, and its three mesh
  tables are non-`static` globals. Including any header that uses `end`
  after that define would break. Keep the macro out of headers.
- `atFunctions.cxx` defines global functions with the ASCA attitude
  library's names (`atSun`, `atSidereal`, …). Linking HEASoft's
  `libatFunctions` into the same binary would cause a symbol clash.

**Unmerged branches** (as of `9c795b4`):
- `origin/f26-cpp17-compliance` (2026-10-16): C++17 cleanups in
  `read_ephem.cxx`, `main.cxx` and others.
- `origin/addition_creator_fits` and `origin/acf_local` (2025-08): put a
  real version in the template's `CREATOR`.
- `origin/Issue17` (2022): optional static libraries.

## Extension points

- **New ephemeris source:** write a reader with the signature
  `EphemData* f(FILE*, double start, double end, double units, double res)`
  that fills a regular grid. Add it to the `EphemFunc` checks in
  `main.cxx`, in `parseInit`, and in the three re-read blocks in
  `makeAttTako`/`makeAttAsFl`. Add it to the par file prompt text.
- **New SINGLE command:** add a `match_str(ini->TLname, "NEWMODE")` branch
  in `doCmd` that fills the `Attitude` (see the `POINTED` branch using
  `MakeAtt2`). Then call `occult`/`doLimbTrace`/`saa` like the other
  branches.
- **New FT2 column or keyword:** add it to `data/ft2.tpl` and set it in
  the write loop in `main.cxx`.
- **Real geomagnetic columns:** compute them with
  `astro::EarthCoordinate` (as `fitsGen`'s `makeFT2` does) in the write
  loop. That would add an `astro` dependency.

## Further reading

- `src/fermitools-fhelp/fhelp_files/gtorbsim.txt`: user help, with
  parameter descriptions and examples.
- `doc/release.notes`: history to 2016.
- `src/observationSim/` (`gtobssim`): the main consumer of simulated FT2
  files. `src/fitsGen/` has the other FT2 writer (`makeFT2`).
- Hoots & Roehrich, *Spacetrack Report No. 3* (SGP4).
- ASCA `atFunctions` (HEASoft attitude library), the source of
  `atFunctions.cxx`.
