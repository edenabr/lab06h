add_executable(equation
    equation.cpp
)

target_link_libraries(equation
    solver
    formatter_ex
)

set_target_properties(equation PROPERTIES OUTPUT_NAME solver)

install(TARGETS equation
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)
