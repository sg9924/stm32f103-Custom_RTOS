# RTOS - Round Robin Scheduler

# Intro:
- simple round robin scheduler
- only MSP is used by both kernel and tasks

# Configurations Present:
- No of Tasks
- Stack Size (Same for all Tasks)



# APIs:
## Kernel:

### 1. rtosKernel_StackInit:

- Inputs:
  - None

- Outputs:
  - None

- Function:
  - to initialize the task for each task TCB
  - Setting bit to force thumb state
  - Setting LR to return from exception to thread mode and use MSP

### 2. rtosKernel_TaskInit:

- Inputs:
  - None

- Outputs:
  - None

- Function:
  - create linked list of tasks (circular)
  - initialize PC and stack for each task
  - initialize current task pointer -> `pcurrent`


### 3. rtosKernel_Launch:

- Inputs:
  - quanta: controls systick rate

- Outputs:
  - None

- Function:
  - configure systick based on quanta value
  - call `rtosKernel_TaskInit`
  - call `rtosScheduler_Launch`


### 4. rtosScheduler_Launch:

- Inputs:
  - None

- Outputs:
  - None

- Function:
  - launch the scheduler to start running the first task