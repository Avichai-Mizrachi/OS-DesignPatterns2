Sure! Here's another version of the description:

## Description
This assignment involves implementing a program that simulates a pipeline of active objects using POSIX threads in C. The pipeline consists of four main components: Task, Queue, Active Object, and Pipeline.

### Task
The Task struct represents a task within the pipeline.

### Queue
The Queue is a thread-safe data structure that allows multiple threads to access it concurrently. It provides blocking operations for enqueueing and dequeueing items.

### Active Object
The Active Object is a separate thread that executes a handler function. It operates within the pipeline and communicates with other active objects through the queue.

### Pipeline
The Pipeline is composed of four active objects that interact with each other using the queue. Each active object performs specific operations on the tasks it receives and passes them along to the next active object in the pipeline.

The pipeline's structure is as follows:
1. **Producer (Active Object 1):** This active object generates random numbers and sends them to the queue of the first consumer.
2. **First Consumer (Active Object 2):** The first consumer receives numbers from the producer, checks if they are prime numbers, adds 11 to them, and sends them to the queue of the second consumer.
3. **Second Consumer (Active Object 3):** The second consumer receives numbers from the first consumer, checks if they are prime numbers, subtracts 13 from them, and sends them to the queue of the third consumer.
4. **Third Consumer (Active Object 4):** The third consumer receives numbers from the second consumer, checks if they are prime numbers, adds 2 to them, and prints them. The printed number should be the same as the number generated by the producer.

By organizing the tasks into a pipeline of active objects, this program effectively processes and transforms data as it flows through the pipeline.

## How to run
```
# Run the program
./st_pipeline
```