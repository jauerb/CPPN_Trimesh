Project(NEAT)

if(CYGWIN)
	ADD_DEFINITIONS(
		-DTIXML_USE_STL
		-DWIN32
		-D_CONSOLE
		-D__WXMSW__
		-DWXUSINGDLL
		-DNOPCH
		-D_USE_MATH_DEFINES
		)
ELSE(CYGWIN)
	if(WIN32)
		ADD_DEFINITIONS(
			-DTIXML_USE_STL
			-DWIN32
			-D_CONSOLE
			-D__WXMSW__
			-DWXUSINGDLL
			-DNOPCH
			-D_USE_MATH_DEFINES
			)
	else(WIN32)
		ADD_DEFINITIONS(
			-DTIXML_USE_STL
			-D__WXGTK__
			-DWXUSINGDLL
			-DNOPCH
			-D_USE_MATH_DEFINES
			-pthread
			)
	endif(WIN32)
endif(CYGWIN)

IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	ADD_DEFINITIONS(-fPIC)
ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

SET(
	USE_GUI 
	OFF
	CACHE 
	BOOL
	"Should we add support for WxWidgets?"
	)

SET(
	BUILD_PYTHON
	OFF 
	CACHE 
	BOOL
	"Build the python extension?"
	)

SET(
	BUILD_MPI
	OFF 
	CACHE 
	BOOL
	"Build the MPI extension?"
	)

IF(USE_GUI)
	REMOVE_DEFINITIONS(-DHCUBE_NOGUI)
ELSE(USE_GUI)
	ADD_DEFINITIONS(-DHCUBE_NOGUI)
ENDIF(USE_GUI)

SET(CMAKE_CXX_FLAGS "${CMAKE_WX_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__WXDEBUG__ -DWXDEBUG=1")

INCLUDE_REGULAR_EXPRESSION( "^.*([Hh][Cc][Uu][Bb][Ee]|[Nn][Ee][Aa][Tt]|main).*$" )

SET(
	WXWIDGETS_INCLUDE
	"../../../../Libraries/wxWidgets-2.8.0/include"
	CACHE
	FILEPATH 
	"WxWidgets include directory"
	)

SET(
	JGTL_INCLUDE
	"../JGTL/include"
	CACHE
	FILEPATH 
	"JGTL include directory"
	) 

SET(
	MPI_INCLUDE_PATH
	"/opt/lam/gnu/include"
	CACHE
	FILEPATH 
	"MPI include directory"
	)

SET(
	ZLIB_INCLUDE
	"../zlib"
	CACHE
	FILEPATH 
	"Zlib include directory"
	)

SET(
	PYTHON_INCLUDE
	"/usr/include"
	CACHE
	FILEPATH 
	"Python include directory"
	)

IF(CYGWIN)
	SET(
		WXWIDGETS_BUILD_INCLUDE
		"../../../../Libraries/wxWidgets-2.8.0/build/cygwin_release/lib/wx/include/msw-unicode-release-2.8"
		CACHE
		FILEPATH 
		"WxWidgets setup.h include directory"
		)
ELSE(CYGWIN)
	IF(WIN32)
		SET(
			WXWIDGETS_BUILD_INCLUDE
			"../../../../Libraries/wxWidgets-2.8.0/lib/vc_dll/msw"
			CACHE
			FILEPATH 
			"WxWidgets setup.h include directory"
			)
	ELSE(WIN32)
		SET(
			WXWIDGETS_BUILD_INCLUDE
			"../../../../Libraries/wxWidgets-2.8.0/build/linux_release/lib/wx/include/gtk2-unicode-release-2.8"
			CACHE
			FILEPATH 
			"WxWidgets setup.h include directory"
			)
	ENDIF(WIN32)
ENDIF(CYGWIN)


INCLUDE_DIRECTORIES(
	include
	../NEAT/include
	${TINYXMLDLL_INCLUDE}
	${BOOST_ROOT}
	${WXWIDGETS_INCLUDE}
	${WXWIDGETS_BUILD_INCLUDE}
	${JGTL_INCLUDE}
	${MPI_INCLUDE_PATH}
	${MPI_INCLUDE}
	${ZLIB_INCLUDE}
	${PYTHON_INCLUDE}
	)

IF(CYGWIN)

	SET(
		WXWIDGETS_DEBUG_LIB
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libraries/wxWidgets-2.8.0/build/cygwin_debug/lib/"
		CACHE
		FILEPATH 
		"WxWidgets Debug Library Directory"
		)

	SET(
		WXWIDGETS_RELEASE_LIB
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libraries/wxWidgets-2.8.0/build/cygwin_release/lib/"
		CACHE
		FILEPATH 
		"WxWidgets Release Library Directory"
		)

ELSE(CYGWIN)
	IF(WIN32)

		SET(
			WXWIDGETS_DEBUG_LIB
			"${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libraries/wxWidgets-2.8.0/lib/vc_dll"
			CACHE
			FILEPATH 
			"WxWidgets Debug Library Directory"
			)

		SET(
			WXWIDGETS_RELEASE_LIB
			"${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libraries/wxWidgets-2.8.0/lib/vc_dll"
			CACHE
			FILEPATH 
			"WxWidgets Release Library Directory"
			)

	ELSE(WIN32)

		SET(
			WXWIDGETS_DEBUG_LIB
			"${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libraries/wxWidgets-2.8.0/build/linux_debug/lib/"
			CACHE
			FILEPATH 
			"WxWidgets Debug Library Directory"
			)

		SET(
			WXWIDGETS_RELEASE_LIB
			"${CMAKE_CURRENT_SOURCE_DIR}/../../../../Libraries/wxWidgets-2.8.0/build/linux_release/lib/"
			CACHE
			FILEPATH 
			"WxWidgets Release Library Directory"
			)

	ENDIF(WIN32)
ENDIF(CYGWIN)

SET(
	MPI_LIB
	"/opt/lam/gnu/lib"
	CACHE
	FILEPATH
	"MPI Library Directory"
	)

SET(
	PYTHON_LIB
	"/usr/lib"
	CACHE
	FILEPATH 
	"Python library directory"
	)

LINK_DIRECTORIES(
	${TINYXMLDLL_LIB}
	${BOOST_STAGE}
	${WXWIDGETS_DEBUG_LIB}
	${WXWIDGETS_RELEASE_LIB}
	${MPI_LIB}
	${PYTHON_LIB}
	${LIBRARY_OUTPUT_PATH}
	)

ADD_LIBRARY(
	Hypercube_NEAT_Base
	STATIC

	src/HCUBE_Defines.cpp
	src/HCUBE_EvaluationPanel.cpp
	src/HCUBE_ExperimentPanel.cpp
	src/HCUBE_ExperimentRun.cpp
	src/HCUBE_EvaluationSet.cpp
	src/HCUBE_MainApp.cpp
	src/HCUBE_MainFrame.cpp
	src/HCUBE_NetworkPanel.cpp
	src/HCUBE_SubstratePanel.cpp
	src/HCUBE_GUITemplate.cpp
	src/HCUBE_ViewIndividualFrame.cpp
	src/HCUBE_UserEvaluationFrame.cpp
	src/HCUBE_NeuralNetworkPanel.cpp
	src/HCUBE_NeuralNetworkWeightGrid.cpp


	src/Experiments/HCUBE_EvolveMorphTrimeshExperiment.cpp	
	src/Experiments/HCUBE_XorExperiment.cpp
	src/Experiments/HCUBE_CheckersCommon.cpp
	src/Experiments/HCUBE_CheckersExperiment.cpp
	src/Experiments/HCUBE_CheckersExperimentOriginalFogel.cpp
	src/Experiments/HCUBE_CheckersExperimentNoGeom.cpp
	src/Experiments/HCUBE_FindClusterExperiment.cpp
	src/Experiments/HCUBE_FindClusterBPExperiment.cpp
	src/Experiments/HCUBE_FindClusterNoGeomExperiment.cpp
	src/Experiments/HCUBE_FindPointExperiment.cpp
	src/Experiments/HCUBE_TicTacToeExperiment.cpp
	src/Experiments/HCUBE_TicTacToeGameExperiment.cpp
	src/Experiments/HCUBE_TicTacToeGameNoGeomExperiment.cpp
	src/Experiments/HCUBE_TicTacToeGameTreeSearch.cpp

	include/HCUBE_Boost.h
	include/HCUBE_Defines.h
	include/HCUBE_EvaluationPanel.h
	include/HCUBE_EvaluationSet.h
	include/HCUBE_ExperimentPanel.h
	include/HCUBE_ExperimentRun.h
	include/HCUBE_MainApp.h
	include/HCUBE_MainFrame.h
	include/HCUBE_NetworkPanel.h
	include/HCUBE_STL.h
	include/HCUBE_SubstratePanel.h
	include/HCUBE_GUITemplate.h
	include/HCUBE_UserEvaluationFrame.h
	include/HCUBE_NeuralNetworkPanel.h
	include/HCUBE_NeuralNetworkWeightGrid.h
	include/HCUBE_ViewIndividualFrame.h


	include/Experiments/HCUBE_EvolveMorphTrimeshExperiment.h		
	include/Experiments/HCUBE_XorExperiment.h
	include/Experiments/HCUBE_Experiment.h
	include/Experiments/HCUBE_CheckersExperiment.h
	include/Experiments/HCUBE_CheckersExperimentOriginalFogel.h
	include/Experiments/HCUBE_CheckersCommon.h
	include/Experiments/HCUBE_CheckersExperimentNoGeom.h
	include/Experiments/HCUBE_FindClusterExperiment.h
	include/Experiments/HCUBE_FindClusterBPExperiment.h
	include/Experiments/HCUBE_FindClusterNoGeomExperiment.h
	include/Experiments/HCUBE_FindPointExperiment.h
	include/Experiments/HCUBE_TicTacToeExperiment.h
	include/Experiments/HCUBE_TicTacToeGameExperiment.h
	include/Experiments/HCUBE_TicTacToeGameNoGeomExperiment.h
	include/Experiments/HCUBE_TicTacToeGameTreeSearch.h
	include/Experiments/HCUBE_TicTacToeShared.h
	)

ADD_DEPENDENCIES(Hypercube_NEAT_Base NEAT)

SET_TARGET_PROPERTIES(Hypercube_NEAT_Base PROPERTIES DEBUG_POSTFIX _d)

ADD_EXECUTABLE(
	Hypercube_NEAT

	src/main.cpp
	)

SET_TARGET_PROPERTIES(Hypercube_NEAT PROPERTIES DEBUG_POSTFIX _d)

IF(CYGWIN)
	TARGET_LINK_LIBRARIES(
		Hypercube_NEAT

		debug
		Hypercube_NEAT_Base_d

		optimized
		Hypercube_NEAT_Base

		debug
		tinyxmldll_d

		optimized
		tinyxmldll

		debug
		NEAT_d

		optimized
		NEAT

		debug
		boost_thread-mt-d

		optimized
		boost_thread-mt

		debug
		boost_filesystem-mt-d

		optimized
		boost_filesystem--mt

		)
ELSE(CYGWIN)
	IF(WIN32)
		TARGET_LINK_LIBRARIES(
			Hypercube_NEAT 

			debug
			Hypercube_NEAT_Base_d

			optimized
			Hypercube_NEAT_Base

			debug
			tinyxmldll_d

			optimized
			tinyxmldll

			debug
			NEAT_d

			optimized
			NEAT

			)
	ELSE(WIN32)
		TARGET_LINK_LIBRARIES(
			Hypercube_NEAT 

			debug
			Hypercube_NEAT_Base_d

			optimized
			Hypercube_NEAT_Base

			debug
			tinyxmldll_d

			optimized
			tinyxmldll

			debug
			NEAT_d

			optimized
			NEAT

			debug
			boost_thread-mt-d

			optimized
			boost_thread-mt

			debug
			boost_filesystem-mt-d

			optimized
			boost_filesystem-mt

			debug
			boost_system-mt

			optimized
			boost_system-mt

			)
	ENDIF(WIN32)
ENDIF(CYGWIN)

IF(USE_GUI)
	IF(CYGWIN)
		TARGET_LINK_LIBRARIES(
			Hypercube_NEAT

			debug
			wx_based-2.8

			optimized
			wx_base-2.8

			debug
			wx_mswd_core-2.8

			optimized
			wx_msw_core-2.8

			debug
			wx_mswd_adv-2.8

			optimized
			wx_msw_adv-2.8
			)
	ELSE(CYGWIN)
		IF(WIN32)
			TARGET_LINK_LIBRARIES(
				Hypercube_NEAT 

				debug
				wxbase28d

				optimized
				wxbase28

				debug
				wxmsw28d_core

				optimized
				wxmsw28_core

				debug
				wxmsw28d_adv

				optimized
				wxmsw28_adv
				)
		ELSE(WIN32)
			TARGET_LINK_LIBRARIES(
				Hypercube_NEAT 

				debug
				wx_baseud-2.8

				optimized
				wx_baseu-2.8

				debug
				wx_gtk2ud_core-2.8

				optimized
				wx_gtk2u_core-2.8

				debug
				wx_gtk2ud_adv-2.8

				optimized
				wx_gtk2u_adv-2.8
				)
		ENDIF(WIN32)
	ENDIF(CYGWIN)
ENDIF(USE_GUI)

ADD_DEPENDENCIES(Hypercube_NEAT NEAT Hypercube_NEAT_Base)

IF(BUILD_MPI)

	ADD_EXECUTABLE(
		Hypercube_NEAT_MPI

		src/HCUBE_MPIExperimentRun.cpp
		src/HCUBE_MPIEvaluationSet.cpp
		src/mpimain.cpp

		include/HCUBE_MPIExperimentRun.h
		include/HCUBE_MPIEvaluationSet.h

		)

	SET_TARGET_PROPERTIES(
		Hypercube_NEAT_MPI 
		PROPERTIES DEBUG_POSTFIX _d
		#COMPILE_FLAGS "-DHCUBE_NOGUI -DHCUBE_USEMPI -DUSE_STDARG -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_UNISTD_H=1 -DHAVE_STDARG_H=1 -DUSE_STDARG=1 -DMALLOC_RET_VOID=1"
		COMPILE_FLAGS "-DHCUBE_USEMPI"
		)

	IF(CYGWIN)
		TARGET_LINK_LIBRARIES(
			Hypercube_NEAT_MPI

			debug
			Hypercube_NEAT_Base_d

			optimized
			Hypercube_NEAT_Base

			debug
			tinyxmldll_d

			optimized
			tinyxmldll

			debug
			NEAT_d

			optimized
			NEAT

			debug
			boost_thread-mt-d

			optimized
			boost_thread-mt

			debug
			boost_filesystem-mt-d

			optimized
			boost_filesystem-mt
			)
	ELSE(CYGWIN)
		IF(WIN32)
			TARGET_LINK_LIBRARIES(
				Hypercube_NEAT_MPI

				debug
				Hypercube_NEAT_Base_d

				optimized
				Hypercube_NEAT_Base

				debug
				tinyxmldll_d

				optimized
				tinyxmldll

				debug
				NEAT_d

				optimized
				NEAT

				mpi
				)
		ELSE(WIN32)
			TARGET_LINK_LIBRARIES(
				Hypercube_NEAT_MPI

				debug
				Hypercube_NEAT_Base_d

				optimized
				Hypercube_NEAT_Base

				debug
				tinyxmldll_d

				optimized
				tinyxmldll

				debug
				NEAT_d

				optimized
				NEAT

				debug
				boost_thread-mt-d

				optimized
				boost_thread-mt

				debug
				boost_filesystem-mt-d

				optimized
				boost_filesystem-mt

				debug
				boost_system-mt

				optimized
				boost_system-mt

				#lammpio

				#lammpi++

				#lamf77mpi

				mpich

				pmpich

				pmpich++

				#lam

				#util

				#dl

				#mpi_cxx 

				#mpi 

				#orte 

				#opal 	

				#dl

				#nsl

				#util
				)
		ENDIF(WIN32)
	ENDIF(CYGWIN)

	IF(USE_GUI)
		IF(CYGWIN)
			TARGET_LINK_LIBRARIES(
				Hypercube_NEAT_MPI

				debug
				wx_based-2.8

				optimized
				wx_base-2.8

				debug
				wx_mswd_core-2.8

				optimized
				wx_msw_core-2.8

				debug
				wx_mswd_adv-2.8

				optimized
				wx_msw_adv-2.8
				)
		ELSE(CYGWIN)
			IF(WIN32)
				TARGET_LINK_LIBRARIES(
					Hypercube_NEAT_MPI

					debug
					wxbase28d

					optimized
					wxbase28

					debug
					wxmsw28d_core

					optimized
					wxmsw28_core

					debug
					wxmsw28d_adv

					optimized
					wxmsw28_adv
					)
			ELSE(WIN32)
				TARGET_LINK_LIBRARIES(
					Hypercube_NEAT_MPI

					debug
					wx_baseud-2.8

					optimized
					wx_baseu-2.8

					debug
					wx_gtk2ud_core-2.8

					optimized
					wx_gtk2u_core-2.8

					debug
					wx_gtk2ud_adv-2.8

					optimized
					wx_gtk2u_adv-2.8
					)
			ENDIF(WIN32)
		ENDIF(CYGWIN)
	ENDIF(USE_GUI)

	ADD_DEPENDENCIES(Hypercube_NEAT_MPI NEAT Hypercube_NEAT_Base)

ENDIF(BUILD_MPI)

IF(BUILD_PYTHON)

	ADD_DEFINITIONS(
		-DHCUBE_USE_BOOST_PYTHON
		)

	ADD_LIBRARY(
		HCUBE
		SHARED

		src/main.cpp
		src/HCUBE_Python.cpp

		)

	SET_TARGET_PROPERTIES(HCUBE PROPERTIES DEBUG_POSTFIX _d)

	SET_TARGET_PROPERTIES(HCUBE PROPERTIES SUFFIX .pyd)

	IF(CYGWIN)
		TARGET_LINK_LIBRARIES(
			HCUBE 

			debug
			Hypercube_NEAT_Base_d

			optimized
			Hypercube_NEAT_Base

			debug
			tinyxmldll_d

			optimized
			tinyxmldll

			debug
			NEAT_d

			optimized
			NEAT

			debug
			boost_thread-mt-d

			optimized
			boost_thread-mt

			debug
			boost_filesystem-mt-d

			optimized
			boost_filesystem-mt

			debug
			wx_based-2.8

			optimized
			wx_base-2.8

			debug
			wx_mswd_core-2.8

			optimized
			wx_msw_core-2.8

			debug
			wx_mswd_adv-2.8

			optimized
			wx_msw_adv-2.8

			python25
			)
	ELSE(CYGWIN)
		IF(WIN32)
			TARGET_LINK_LIBRARIES(
				HCUBE 

				debug
				Hypercube_NEAT_Base_d

				optimized
				Hypercube_NEAT_Base

				debug
				tinyxmldll_d

				optimized
				tinyxmldll

				debug
				NEAT_d

				optimized
				NEAT

				#debug
				#boost_thread-vc80-mt-gd-1_35_1

				#optimized
				#boost_thread-vc80-mt-1_35_1

				#debug
				#boost_filesystem-vc80-mt-gd-1_35_1

				#optimized
				#boost_filesystem-vc80-mt-1_35_1

				debug
				wxbase28d

				optimized
				wxbase28

				debug
				wxmsw28d_core

				optimized
				wxmsw28_core

				debug
				wxmsw28d_adv

				optimized
				wxmsw28_adv

				python25
				)
		ELSE(WIN32)
			TARGET_LINK_LIBRARIES(
				HCUBE 

				debug
				Hypercube_NEAT_Base_d

				optimized
				Hypercube_NEAT_Base

				debug
				tinyxmldll_d

				optimized
				tinyxmldll

				debug
				NEAT_d

				optimized
				NEAT

				debug
				boost_thread-mt-d

				optimized
				boost_thread-mt

				debug
				boost_filesystem-mt-d

				optimized
				boost_filesystem-mt

				debug
				boost_system-mt


				optimized
				boost_system-mt

				debug
				wx_baseud-2.8

				optimized
				wx_baseu-2.8

				debug
				wx_gtk2ud_core-2.8

				optimized
				wx_gtk2u_core-2.8

				debug
				wx_gtk2ud_adv-2.8

				optimized
				wx_gtk2u_adv-2.8

				python2.5
				)
		ENDIF(WIN32)
	ENDIF(CYGWIN)

	ADD_DEPENDENCIES(HCUBE NEAT Hypercube_NEAT_Base)

ENDIF(BUILD_PYTHON)

