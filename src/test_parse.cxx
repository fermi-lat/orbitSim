include "timeline.h"

int readout(InitI *ini, EphemData *ephem) {
  try{
    std::cout << "-------------------------\n";
    std::cout << "Attempting to readout parsed data...\n";
    std::cout << "-------------------------\n";
    std::cout << "Timeline filename: " << ini->timeline.filename << std::endl;
    std::cout << "Timeline creation time: " << ini->timeline.creation_time << std::endl;
    std::cout << "Timeline mission id: " << ini->timeline.mission_id << std::endl;
    std::cout << "Timeline originator: " << ini->timeline.originator << std::endl;
    std::cout << "Timeline database version: " << ini->timeline.db_version << std::endl;
    std::cout << "Timeline destination processor: " << ini->timeline.dest_processor << std::endl;
    std::cout << "Timeline start time: " << ini->timeline.start_time << std::endl;
    std::cout << "Timeline stop time: " << ini->timeline.stop_time << std::endl;
    std::cout << "Timeline execute flag: " << ini->timeline.execute_flag << std::endl;
    std::cout << "Timeline type: " << ini->timeline.timeline_type << std::endl;
    std::cout << "Timeline version number: " << ini->timelime.version_num << std::endl;
    std::cout << "Timeline reference file: " << ini->timeline.ref_timeline_name << std::endl;
    std::cout << "Timeline comments: " << ini->timeline.comment << std::endl;
    std::cout << "-------------------------\n" << std::endl;
    std::cout << "SUCESSFULLY PARSED TIMELINE!" << std::endl;
  } catch {
    std::cerr << "Reading the timeline attribute failed!" << std::endl;
    return 1;
  }
}

int main(int argc, char **argv) {
  char const* filename;
  if (argc>1) {
    filename = argv[1];
  }
  else {
    std::cerr << "Error: No input file provided." << std::endl;
    return 1;
  }

  
  
}
