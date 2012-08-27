
# Flex Turbine+ADLB for steals with imbalanced task loads

# usage: bin/turbine -l -n 6 tests/adlb-steal-2.tcl

# Test will add tasks up until time exceeds DURATION
# Task run times are from 0 to max_task
# When a worker completes a task, it may submit new tasks

package require turbine 0.0.1

proc default { env_var d } {
    global env
    if [ info exists env($env_var) ] {
        set result $env($env_var)
    } else {
        set result $d
    }
    return $result
}

# Initial puts to bootstrap
set task_count_initial 100

# Maximal task length (seconds)
set task_length_max 0

# Probability of releasing new work
set task_chance 0

# Maximal number of new tasks found
set task_count_max 0

# Seed for srand().  rank is added before using
set rand_seed 1

set servers [ default ADLB_SERVERS 1 ]

# duration in seconds
set duration [ default TEST_DURATION 0 ]

enum WORK_TYPE { T }

# "seconds" or "milliseconds"
set clock_resolution "milliseconds"
# In seconds.  Is a float if clock_resolution==milliseconds
set clock_start 0

proc clock_init { } {
    global clock_resolution
    global clock_start
    set clock_start [ clock $clock_resolution ]
}

# Walltime since this script started (clock_init) (in seconds)
proc clock_wt { } {
    global clock_resolution
    global clock_start
    set t [ clock $clock_resolution ]
    set d [ expr $t - $clock_start ]
    if { [ string equal $clock_resolution "milliseconds" ] } {
        set d [ expr double($d) / 1000 ]
    }
    return $d
}

proc clock_fmt { } {
    global clock_resolution
    if { [ string equal $clock_resolution "seconds" ] } {
        return format "%4i" [ clock_wt ]
    } else {
        return format "%7.3f" [ clock_wt ]
    }
}

proc clock_report { } {
    set d [ clock_fmt ]
    puts "clock: $d"
}

proc log { msg } {
    # puts "LOG: [clock_fmt] $msg"
}

# Obtain a random task length
proc random_task { } {
    global task_length_max
    return [ expr round(rand() * $task_length_max) ]
}

# Return count of new work units
# Based on probability of work unit finding additional work
#  and
proc found_work { } {
    global duration
    global task_chance
    global task_count_max
    set t [ clock_wt ]
    if { $t < $duration } {
        if { rand() < $task_chance } {
            return $task_count_max
        }
    }
    return 0
}

proc put_found_work { } {
    global WORK_TYPE
    set new_tasks [ found_work ]
    log "new tasks: $new_tasks"
    for { set i 0 } { $i < $new_tasks } { incr i } {
        adlb::put $adlb::RANK_ANY $WORK_TYPE(T) [random_task] 0
    }
}

adlb::init $servers [ array size WORK_TYPE ]

clock_init

set amserver [ adlb::amserver ]

set rank [ adlb::rank ]

set tasks_run 0

expr srand($rand_seed + $rank)

if { $amserver == 0 } {

    if { $rank == 0 } { clock_report }

    if { $rank == 0 } {
        for { set i 0 } { $i < $task_count_initial } { incr i } {
            # clock_report
            adlb::put $adlb::RANK_ANY $WORK_TYPE(T) [random_task] 0
        }
    }
    # after 1000
    # log "worker starting"
    while 1 {
        # clock_report
        set msg [ adlb::get $WORK_TYPE(T) answer_rank ]
        # log "msg: '$msg'"
        if { [ string length $msg ] == 0 } break
        # log "answer_rank: $answer_rank"
        # Emulate work time
        # log "work unit start"
        after [ expr $msg * 1000 ]
        # log "work unit stop"
        put_found_work
        incr tasks_run
    }
} else {
    adlb::server
}

if { $rank == 0 } { clock_report }

adlb::finalize
# puts OK

puts "tasks_run: $tasks_run"

proc exit args {}
