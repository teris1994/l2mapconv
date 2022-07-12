set(TARGETS
    app
)

foreach(TARGET ${TARGETS})
    add_subdirectory(${TARGET})

    set_target_properties(${TARGET} PROPERTIES ${TARGET_PROPERTIES})
    target_compile_options(${TARGET} PRIVATE ${TARGET_COMPILE_OPTIONS})
endforeach()
