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

@test "Check single pipe (ls | grep dshlib.c)" {
    run ./dsh <<EOF
ls | grep dshlib.c
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "dshlib.c" ]]
}

@test "Check multiple pipes (ls | grep dsh | wc -l)" {
    run ./dsh <<EOF
ls | grep dsh | wc -l
EOF
    [ "$status" -eq 0 ]
}

@test "Check empty input" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
}

@test "Check whitespace trimming" {
    run ./dsh <<EOF
   echo trimmed
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "trimmed" ]]
}

@test "Check command with quotes" {
    run ./dsh <<EOF
echo "hello bats"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "hello bats" ]]
}
