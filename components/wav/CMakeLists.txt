idf_component_register(SRCS "wav.c"
                       INCLUDE_DIRS ".")
set_source_files_properties(wav.c COMPILE_OPTIONS -Wno-multichar)
