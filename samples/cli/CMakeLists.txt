if(${WITH_RTSPDEMO_CLI})
	add_subdirectory ("rtspdemo")
endif()

if(${WITH_PUSHFRAMEDEMO_CPP_CLI})
	add_subdirectory ("push_frame_demo_cpp")
endif()

if(${WITH_PUSHFRAMEDEMO_C_CLI})
	add_subdirectory ("push_frame_demo_c")
endif()
