#!/bin/bash

 

debugfs=/sys/kernel/debug

echo nop > $debugfs/tracing/current_tracer

echo 0 > $debugfs/tracing/tracing_on

echo `pidof unlink` > $debugfs/tracing/set_ftrace_pid

echo function_graph > $debugfs/tracing/current_tracer

echo vfs_unlink > $debugfs/tracing/set_graph_function

echo 1 > $debugfs/tracing/tracing_on
