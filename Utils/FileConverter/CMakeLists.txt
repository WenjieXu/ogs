
INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/Utils/FileConverter
	${CMAKE_SOURCE_DIR}/BaseLib
	${CMAKE_SOURCE_DIR}/GeoLib
	${CMAKE_SOURCE_DIR}/FileIO
	${CMAKE_SOURCE_DIR}/MathLib
	${CMAKE_SOURCE_DIR}/MeshLib
	${CMAKE_SOURCE_DIR}/Gui/OpenSG
)

IF (QT4_FOUND)
	ADD_EXECUTABLE( ConvertSHPToGLI ConvertSHPToGLI.cpp )
	SET_TARGET_PROPERTIES(ConvertSHPToGLI PROPERTIES FOLDER Utilities)

	TARGET_LINK_LIBRARIES( ConvertSHPToGLI
		FileIO
		GeoLib
		BaseLib
		OgsLib
		shp
		${QT_LIBRARIES}
	)

	INSTALL ( TARGETS ConvertSHPToGLI RUNTIME DESTINATION bin COMPONENT ConvertSHPToGLI )

	ADD_EXECUTABLE (generateBCandGLI generateBCandGLI.cpp )
	TARGET_LINK_LIBRARIES (generateBCandGLI
		FileIO
		GeoLib
		OgsLib
		${QT_LIBRARIES}
	)

	ADD_EXECUTABLE (generateBCFromPolyline generateBCFromPolyline.cpp )
	TARGET_LINK_LIBRARIES (generateBCFromPolyline
		FileIO
		GeoLib
		OgsLib
		${QT_LIBRARIES}
	)

	SET_TARGET_PROPERTIES(generateBCandGLI generateBCFromPolyline
		PROPERTIES FOLDER Utilities)
ENDIF () # QT4_FOUND

IF (Boost_FOUND)
	ADD_EXECUTABLE (generateMatPropsFromMatID generateMatPropsFromMatID.cpp )
	TARGET_LINK_LIBRARIES (generateMatPropsFromMatID
		FileIO
		MeshLib
		logog
		${Boost_LIBRARIES}
	)
	SET_TARGET_PROPERTIES(generateMatPropsFromMatID
		PROPERTIES FOLDER Utilities)
ENDIF () # Boost_FOUND

ADD_EXECUTABLE (GMSH2OGS GMSH2OGS.cpp)
SET_TARGET_PROPERTIES(GMSH2OGS PROPERTIES FOLDER Utilities)
TARGET_LINK_LIBRARIES (GMSH2OGS
	MeshLib
	FileIO
)
IF(MSVC)
	TARGET_LINK_LIBRARIES (GMSH2OGS winmm)
ENDIF()

ADD_EXECUTABLE (OGS2VTK OGS2VTK.cpp)
SET_TARGET_PROPERTIES(OGS2VTK PROPERTIES FOLDER Utilities)
TARGET_LINK_LIBRARIES (OGS2VTK
	MeshLib
	FileIO
	zlib
)

ADD_EXECUTABLE (VTK2OGS VTK2OGS.cpp)
SET_TARGET_PROPERTIES(VTK2OGS PROPERTIES FOLDER Utilities)
TARGET_LINK_LIBRARIES (VTK2OGS
	MeshLib
	FileIO
	zlib
)
