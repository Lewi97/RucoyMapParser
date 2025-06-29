add_executable (${PROJECT_NAME} "main.cpp"  "Parser.h" "Parser.cpp" "ByteStream.h" "Parser115.h" "TextureParser.h" "Metrics.h" "Parser118.h" "Parser118.cpp" "MusicRegion.h")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_link_libraries(${PROJECT_NAME}
	PRIVATE pngpp::pngpp
)
