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

@test "Start the server" {
    ./dsh -s &
    sleep 1
    netstat -tlnp | grep 1234
    [ $? -eq 0 ]
}

@test "Execute 'ls' command" {
    ./dsh -s &
    sleep 1
    output=$(echo "ls" | ./dsh -c)
    echo "$output" | grep "rsh_server.c"
    [ $? -eq 0 ]
}

@test "Execute 'pwd' command" {
    ./dsh -s &
    sleep 1
    output=$(echo "pwd" | ./dsh -c)
    echo "$output" | grep "/Users/patrickutama/cs283/cs283/HW6"
    [ $? -eq 0 ]
}


@test "Execute 'ls | grep dsh' remotely" {
    ./dsh -s &
    sleep 1
    output=$(echo "ls | grep dsh" | ./dsh -c)
    echo "$output" | grep "dsh"
    [ $? -eq 0 ]
}

@test "Exit command closes client" {
    ./dsh -s &
    sleep 1
    echo "exit" | ./dsh -c
    sleep 1
    ! ps aux | grep "[d]sh -c"
}

@test "Stop-server command shuts down server" {
    ./dsh -s &
    sleep 1
    echo "stop-server" | ./dsh -c
    sleep 1
    ! ps aux | grep "[d]sh -s"
}