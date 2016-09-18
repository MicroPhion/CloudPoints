cmake_minimum_required(VERSION 2.8)
PROJECT(CloudPoints)

ADD_DEFINITIONS(-D_AFXDLL)
SET(CMAKE_MFC_FLAG 6)
            
       
FILE(GLOB CloudPoints_SRCS  *.cpp)
FILE(GLOB CloudPoints_HEADERS  *.h)

FILE(GLOB CloudPoints_ply ply/ply.c ply/ply.h)
SOURCE_GROUP(Ply FILES ${CloudPoints_ply})   

 
#FIND ITK

FIND_PACKAGE(ITK)
IF(ITK_FOUND)
	INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
	MESSAGE(FATAL_ERROR "Cannot build without ITK, Please set ITK_DIR")
ENDIF(ITK_FOUND)

#FIND VTK
FIND_PACKAGE(VTK)
IF(VTK_FOUND)
	INCLUDE(${VTK_USE_FILE})
ELSE(VTK_FOUND)
	MESSAGE(FATAL_ERROR "Cannot build without VTK, Please set VTK_DIR")
ENDIF(VTK_FOUND)

INCLUDE_DIRECTORIES(

	#${VTKITK_SOURCE_DIR} 
	.\\

) 
ADD_EXECUTABLE(CloudPoints
				${CloudPoints_SRCS} 
				${CloudPoints_HEADERS}
				${CloudPoints_ply}
				)
TARGET_LINK_LIBRARIES(CloudPoints 
						${ITK_LIBRARIES} 
						${VTK_LIBRARIES}
						${ALL_LIBS}
						)
FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

#Visualization Property Pages==>Configuration Property==>C/C++==>General==>Multi-processor Compilation
set_target_properties( Visualization PROPERTIES COMPILE_FLAGS "/MP")






