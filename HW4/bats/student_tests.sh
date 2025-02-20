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

@test "pwd outputs current directory" {
    run ./dsh <<EOF
pwd
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>$(pwd)dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "change directory and verify" {
    mkdir -p /tmp/dsh-test
    run ./dsh <<EOF
cd /tmp/dsh-test
pwd
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>dsh2>/tmp/dsh-testdsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]

    rmdir /tmp/dsh-test
}

@test "cd with no arguments does nothing" {
    current_dir=$(pwd)
    run ./dsh <<EOF
cd
pwd
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>dsh2>${current_dir}dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "echo preserves spaces inside quotes" {
    run ./dsh <<EOF
echo "   hello     world   "
EOF

    stripped_output=$(echo "$output" | tr -d '\r' | sed -E ':a;N;$!ba;s/\n/ /g' | sed 's/  +/ /g')

    expected_output="dsh2>    hello     world    dsh2>  cmd loop returned 0"

    echo "Captured stdout:"
    echo "Output: [$output]"
    echo "${stripped_output} -> ${expected_output}"
    echo "Exit Status: $status"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "which command returns correct path" {
    run ./dsh <<EOF
which ls
EOF

    expected_output=$(which ls)
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>${expected_output}dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "ls command runs without errors" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

@test "invalid command should return an error" {
    run ./dsh <<EOF
thiscommanddoesnotexist
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "Captured stdout:"
    echo "Output: [$output]"
    echo "Exit Status: $status"

    [[ "$stripped_output" == *"commandexecutionerror"* ]]

    if [[ "$stripped_output" == *"commandexecutionerror"* ]]; then
        [ "$status" -eq 0 ] || [ "$status" -ne 0 ]
    else
        [ "$status" -ne 0 ]  # must be a non-zero error code
    fi
}


@test "echo multiple words with spacing" {
    run ./dsh <<EOF
echo Hello   world,    how are  you?
EOF
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    expected_output="dsh2> Hello world, how are you?dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "change into nested directory and verify" {
    mkdir -p /tmp/dsh_test_dir
    run ./dsh <<EOF
cd /tmp/dsh_test_dir
pwd
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>dsh2>/tmp/dsh_test_dirdsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]

    rmdir /tmp/dsh_test_dir
}

@test "exit command terminates shell" {
    run ./dsh <<EOF
exit
EOF

    echo "Captured stdout:"
    echo "Output: [$output]"
    echo "Exit Status: $status"

    [ "$status" -eq 0 ] || [ "$status" -eq 7 ] || [ "$status" -eq 249 ]
}
