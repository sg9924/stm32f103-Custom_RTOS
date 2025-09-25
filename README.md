# Repo Description:
- Custom RTOS for STM32 Blue Pill using Custom Device Drivers, No Third Party Libraries like HAL or CMSIS
- This Repo has been structured in such a way that with each version, we can see the improvements/features added instead of a single project which has the latest version of code only, for learning purposes.


# Intro:
- For now the scheduler currently implemented is a Round Robin Scheduler with Cooperative Scheduling.
- A fixed priority pre-emptive periodic scheduler will be added in later.


# Versions:
- Versions 1-4 all use MSP & SysTick Handler only
- PSP & PendSV Handler functionality will be introduced in the next few versions

## Round Robin v1:
- simplest form of Round Robin Scheduler
- use MSP only for both Kernel and Tasks
- Task only has,
  - A Pointer to the Task Function
- TCBs as a circular linked list
- On Context Switch, the next node (task) is selected


## Round Robin v2:
- **Added Task Yield for Cooperative Scheduling**
- Added Description to Task
- Bringing Task related code into Task source from Kernel source
- Refining Stack Init by adding Task Stack Init and Task Init APIs
- Added Macro for global IRQ Disabling and Enabling
- Added Periodic Tasks but removed it later in this version
- Added Round Robin Scheduler API
- Modified Systick Handler to call the scheduler API
- Added RTOS Info
- Added Config for Scheduler type
- Updated Serialprint


## Round Robin v3:
- **Introduce Semaphores**,
  - Init
  - Give
  - Take
  - Cooperative Take
- Updated Serialprint and done the same to previous versions
- Added Idle Task & Task Delay
- Added Assert
- Modified Round Robin Scheduler to account for the Idle Task and Task Delay Functionality


## Round Robin v4:
- **Introduce Weighted Round Robin Scheduler API**
- Added for tasks,
  - Weighted Task Add API
  - Task Reset Quota API
- Added RTOS Scheduler Init API
- Refine RTOS Info




# Structure:
- Systick is initialized based on the Task Quanta (in milliseconds) defined in Round Robin Scheduler

## Config:
- Settings for the RTOS

## Kernel:
- Main part of the RTOS
- Handles,
  - Initializing TCB's, their stack, the Scheduler
  - Starting/Launching the Scheduler
  - Determining the next task to run
  - Context Switch to the next task

## Tasks:
- Defines the TCB Structure
- API's related to tasks
  - Adding a task
  - Defining a Idle Task
  - Task Yield
  - Task Delay
  - Task Unblock
- TCBs as circular linked list
- TCBs' stack as an array

## RTOS Info:
- provides the necessary info about the tasks and the scheduler before the scheduler starts

## RTOS Port:
- contains the Device Driver Headers
- custom, no HAL or CMSIS

## Semaphore
- Semaphore Definitions which can be used in the tasks