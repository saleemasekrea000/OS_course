#!/bin/bash
# Compile agent and controller if not already compiled
gcc agent.c -o agent
gcc controller.c -o controller
# Start the agent program in the background
./agent &
# Sleep for a moment to ensure the agent is started
sleep 1
./controller
