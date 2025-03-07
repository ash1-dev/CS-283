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
    [[ "$output" == "dsh3>"* ]]
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

@test "ls grep pipe" {
    run "./dsh" <<EOF                
ls | grep dshlib.c
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dshlib.cdsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "echo pipe with wc" {
    run ./dsh <<EOF
echo "a b c" | wc -w
EOF
    [[ "$output" == *"3"* ]]
    [ "$status" -eq 0 ]
}