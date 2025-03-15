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

@test "echo pipe with wc" {
    run ./dsh <<EOF
echo "a b c" | wc -w
EOF
    [[ "$output" == *"3"* ]]
    [ "$status" -eq 0 ]
}

setup() {
    ./dsh -s -i 0.0.0.0 -p 12345 &
    SERVER_PID=$!
    sleep 1 
}

teardown() {
    ./dsh -c -i 127.0.0.1 -p 12345 <<< "stop-server" || true
    wait $SERVER_PID 2>/dev/null || true
}

@test "Server starts and stops gracefully" {
    run nc -z 0.0.0.0 12345
    [ "$status" -eq 0 ]

    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
stop-server
EOF
    [ "$status" -eq 0 ]
}

@test "Basic remote command execution" {
    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
ls
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"dshlib.c"* ]]
    [[ "$output" == *"rsh_server.c"* ]]
}

@test "Remote pipeline command" {
    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
echo "a b c" | wc -w
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"3"* ]]
}

@test "Built-in exit command (client side)" {
    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Built-in cd command" {
    mkdir -p test_dir

    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
cd test_dir
pwd
EOF
    rmdir test_dir
    
    [ "$status" -eq 0 ]
    [[ "$output" == *"test_dir"* ]]
}

@test "Multiple sequential clients" {
    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
echo "First client"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"First client"* ]]

    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
echo "Second client"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"Second client"* ]]
}

@test "Command with quotes" {
    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
echo "Hello   World"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello   World"* ]]
}

@test "Directory operations" {
    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
mkdir remote_test_dir
EOF
    [ "$status" -eq 0 ]
    [ -d "remote_test_dir" ]

    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
rm -r remote_test_dir
EOF
    [ "$status" -eq 0 ]
    [ ! -d "remote_test_dir" ]
}

@test "Large output handling" {
    run ./dsh -c -i 127.0.0.1 -p 12345 <<EOF
cat /etc/services
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"ssh"* ]] 
    [[ "$output" == *"http"* ]]
}