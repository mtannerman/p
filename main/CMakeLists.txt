project(ParserMain)

include_directories(../testutil/inc)
include_directories(../csvparser/inc)


ADD_EXECUTABLE(ParserMain main.cpp)

TARGET_LINK_LIBRARIES(ParserMain testutil csvparser)