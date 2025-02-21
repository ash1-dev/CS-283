#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Basic command: ls" {
    run ./dsh <<EOF                
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Basic command: echo" {
    run ./dsh <<EOF                
echo Hello, world!
EOF
    [[ "$output" == *"Hello, world!"* ]]
}

@test "Basic command: pwd" {
    run ./dsh <<EOF                
pwd
EOF
    [[ "$output" == "$(pwd)"* ]]
}

@test "Basic command: which" {
    run ./dsh <<EOF                
which ls
EOF
    [[ "$output" == "/bin/ls"* ]] || [[ "$output" == "/usr/bin/ls"* ]]
}

@test "Built-in cd: no arguments (should not change directory)" {
    run ./dsh <<EOF                
cd
pwd
EOF
    [[ "$output" == "$(pwd)"* ]]
}

@test "Built-in cd: invalid directory" {
    run ./dsh <<EOF                
cd /invalid_directory
EOF
    [[ "$output" == *"dsh: No such file or directory"* ]]
}

@test "Error: unknown command" {
    run ./dsh <<EOF                
invalidcommand
EOF
    [[ "$output" == *"dsh: command not found: invalidcommand"* ]]
}

@test "Error: missing command" {
    run ./dsh <<EOF                

EOF
    [[ "$output" == "dsh2>"* ]]
}

@test "Quoted string: echo \"hello   world\"" {
    run ./dsh <<EOF                
echo "hello   world"
EOF
    [[ "$output" == *"hello   world"* ]]
}

@test "Quoted string: mixed arguments" {
    run ./dsh <<EOF                
echo Hello "beautiful world"
EOF
    [[ "$output" == *"Hello beautiful world"* ]]
}

@test "Built-in exit command" {
    run ./dsh <<EOF                
exit
EOF
    [ "$status" -eq 0 ]
}
