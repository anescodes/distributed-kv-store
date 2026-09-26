# Distributed Networked Key-Value Store in C

A learning-oriented distributed systems project implemented in **C**, designed to explore how independent processes communicate over a network, distribute data, handle failures, and remain reliable under concurrent workloads.

The project also contains a **controlled security laboratory** demonstrating why secure programming is especially important when developing networked software in low-level languages such as C.

> ⚠️ **Educational Security Warning**
>
> This repository may contain intentionally vulnerable code for educational purposes.
> The vulnerable components are designed to run only in a controlled local laboratory environment.
> Do **not** deploy the vulnerable version on a public server or expose it to networks you do not control.

---

## 🎯 Purpose

The main purpose of this project is to understand the foundations of **Distributed Systems** by building one from the ground up rather than relying on existing distributed databases or frameworks.

The project focuses on four major areas:

### 1. Distributed Systems

Understanding how multiple independent machines/processes cooperate to provide one logical service.

Topics include:

* Distributed nodes
* Data partitioning
* Replication
* Node discovery
* Failure detection
* Recovery
* Consistency
* Fault tolerance
* Distributed coordination

### 2. Network Communication

Understanding how nodes communicate with each other.

Topics include:

* TCP/IP
* Sockets
* Client-server architecture
* Node-to-node communication
* Custom application protocols
* Message serialization
* Request/response systems
* Connection management
* Concurrent connections

### 3. Systems Programming in C

The project is intentionally implemented in C to understand what happens at a lower level.

Topics include:

* Memory management
* Pointers
* Structs
* Dynamic allocation
* Processes
* Threads
* File descriptors
* Sockets
* Synchronization
* Error handling
* Memory safety

### 4. Secure Programming

Networked software can become extremely dangerous when low-level memory bugs are combined with remotely reachable input.

The security laboratory demonstrates this concept using intentionally vulnerable programs.

Topics include:

* Secure input validation
* Buffer boundaries
* Memory corruption
* Stack memory
* Heap memory
* Integer-related bugs
* Unsafe parsing
* Use-after-free
* Crash analysis
* Debugging
* Binary analysis
* Security mitigations

---

# 🏗️ Architecture

The system is designed around multiple independent nodes.

```text
                         ┌──────────────────┐
                         │      Client      │
                         └────────┬─────────┘
                                  │
                                  ▼
                         ┌──────────────────┐
                         │ Gateway / Router │
                         └────────┬─────────┘
                                  │
                  ┌───────────────┼───────────────┐
                  │               │               │
                  ▼               ▼               ▼
           ┌───────────┐   ┌───────────┐   ┌───────────┐
           │  Node 1   │◄─►│  Node 2   │◄─►│  Node 3   │
           └───────────┘   └───────────┘   └───────────┘
                 ▲               ▲               ▲
                 └───────────────┴───────────────┘
                         Node Communication
```

Each node represents an independent process with its own local storage.

Nodes must communicate with each other to:

* Locate data
* Forward requests
* Replicate information
* Detect failures
* Recover from failures
* Maintain distributed state

---

# 🗄️ Key-Value Store

The basic storage model is:

```text
KEY → VALUE
```

Example:

```text
"user:42" → "Anes"
"language" → "C"
"course" → "Distributed Systems"
```

The system initially supports operations such as:

```text
SET key value
GET key
UPDATE key value
DELETE key
```

The first implementation is intentionally simple before introducing distributed behavior.

---

# 🚀 Development Roadmap

## Phase 1 — Local Key-Value Store

Build a single-node storage engine.

Learn:

* Hash tables
* Dynamic memory
* Structs
* Pointers
* Memory allocation
* Basic data structures

---

## Phase 2 — TCP Networking

Turn the local application into a network service.

Learn:

* `socket()`
* `bind()`
* `listen()`
* `accept()`
* `connect()`
* `send()`
* `recv()`
* TCP connections
* Client/server architecture

Example:

```text
Client
   │
   │ TCP
   ▼
Server
   │
   ▼
Hash Table
```

---

## Phase 3 — Multiple Nodes

Introduce multiple independent processes.

Example:

```text
Client
  │
  ▼
Node 1
  │
  ├──────────► Node 2
  │
  └──────────► Node 3
```

Learn:

* Node identification
* Node discovery
* Node-to-node protocols
* Data distribution
* Request forwarding
* Distributed state

---

## Phase 4 — Data Distribution

Implement a mechanism for deciding which node stores a particular key.

Potential approach:

```text
hash(key) → node
```

Later, the project can explore:

* Hash-based partitioning
* Consistent hashing
* Rebalancing
* Node membership changes

---

## Phase 5 — Replication

Store copies of important data on multiple nodes.

Example:

```text
             ┌─────────────┐
             │    SET      │
             │ user:42     │
             │    Anes     │
             └──────┬──────┘
                    │
              ┌─────┴─────┐
              ▼           ▼
           Node 1       Node 2
           Primary      Replica
```

Learn:

* Replication
* Redundancy
* Failure tolerance
* Recovery
* Data consistency

---

## Phase 6 — Failure Detection

Nodes should detect when another node becomes unavailable.

Example:

```text
Node 1 ─── heartbeat ───► Node 2
Node 1 ◄── heartbeat ──── Node 2

Node 2 crashes

Node 1 ─── heartbeat ───► Node 2
Node 1 ─── timeout ─────► Node 2 ❌
```

Learn:

* Heartbeats
* Timeouts
* Failure detection
* Node states
* Recovery mechanisms

---

## Phase 7 — Concurrency

Support multiple clients simultaneously.

Potential implementation:

```text
             Server
                │
       ┌────────┼────────┐
       ▼        ▼        ▼
    Thread 1 Thread 2 Thread 3
       │        │        │
       └────────┼────────┘
                ▼
          Shared Storage
```

Learn:

* Threads
* Mutexes
* Race conditions
* Synchronization
* Concurrent network programming

---

# 🔐 Security Laboratory

The repository also contains a separate security laboratory.

The purpose is **not** to attack external systems.

The purpose is to understand what can happen when vulnerable C code processes untrusted network input.

```text
                    Security Lab

                  Vulnerable Program
                         │
                         ▼
                  Unexpected Input
                         │
                         ▼
                       Crash
                         │
                         ▼
                    Debugging
                         │
                         ▼
                  Memory Analysis
                         │
                         ▼
                    Vulnerability
                     Analysis
                         │
                         ▼
                       Patch
```

The laboratory may contain intentionally vulnerable examples such as:

* Stack buffer overflows
* Unsafe string handling
* Integer boundary errors
* Heap memory bugs
* Use-after-free
* Unsafe protocol parsing

These examples are isolated from the production/secure implementation.

---

# 🧪 Secure vs Vulnerable Implementation

The project should maintain a clear distinction between:

```text
                PROJECT
                   │
        ┌──────────┴──────────┐
        ▼                     ▼
   Secure Version        Security Lab
        │                     │
        │                 Vulnerable
        │                 Components
        │                     │
        ▼                     ▼
 Production-style       Controlled
 implementation        experimentation
```

The vulnerable implementation exists to demonstrate how bugs happen and how they can be detected and fixed.

---

# ⚠️ Why Secure Code Matters

A memory bug in a normal local application is already a serious programming problem.

A memory bug in a **network-facing C application** can become significantly more dangerous because an attacker may be able to provide the input that reaches the vulnerable code.

For example:

```text
Untrusted Network Input
          │
          ▼
     Network Parser
          │
          ▼
     Vulnerable C Code
          │
          ▼
     Memory Corruption
          │
          ▼
       Crash / Security Impact
```

This is why network programming and secure programming cannot be treated as completely separate subjects.

Every network input should be considered **untrusted** until it has been validated.

---

# 🛡️ Security Principles

The secure implementation will follow principles such as:

* Validate all external input
* Never trust client-provided lengths
* Check buffer boundaries
* Check return values
* Handle allocation failures
* Avoid unsafe string operations
* Avoid integer overflow
* Properly manage memory ownership
* Synchronize shared resources
* Minimize unnecessary privileges
* Separate parsing from processing
* Fail safely

The project will also investigate compiler and operating-system protections such as:

* Stack canaries
* ASLR
* NX
* PIE
* RELRO
* Compiler hardening options

---

# 🔬 Reverse Engineering & Debugging

The security laboratory can be analyzed using tools such as:

* GDB
* Ghidra
* objdump
* readelf
* strings
* Linux debugging tools

The learning cycle is:

```text
SOURCE CODE
     │
     ▼
COMPILED BINARY
     │
     ▼
RUN PROGRAM
     │
     ▼
OBSERVE FAILURE
     │
     ▼
DEBUG
     │
     ▼
ANALYZE MEMORY
     │
     ▼
UNDERSTAND ROOT CAUSE
     │
     ▼
PATCH
     │
     ▼
VERIFY FIX
```

The objective is to understand **why** a vulnerability exists, not simply to make a program crash.

---

# 📊 Performance Evaluation

The system will eventually be benchmarked using measurements such as:

* Request latency
* Throughput
* Number of concurrent clients
* CPU usage
* Memory usage
* Network overhead
* Recovery time
* Node failure behavior

Example:

```text
Clients
   │
   ├──── Request ────►
   ├──── Request ────►
   ├──── Request ────►
   │
   ▼
Distributed System
   │
   ▼
Performance Metrics
```

---

# 📁 Proposed Repository Structure

```text
distributed-kv-store/
│
├── README.md
├── LICENSE
│
├── docs/
│   ├── architecture.md
│   ├── protocol.md
│   ├── distributed-design.md
│   └── security.md
│
├── src/
│   ├── client/
│   ├── server/
│   ├── node/
│   ├── storage/
│   ├── network/
│   └── common/
│
├── include/
│   ├── storage.h
│   ├── network.h
│   ├── node.h
│   └── protocol.h
│
├── tests/
│   ├── unit/
│   ├── integration/
│   └── distributed/
│
├── benchmarks/
│
├── security_lab/
│   ├── vulnerable/
│   ├── analysis/
│   └── patches/
│
├── scripts/
│
├── Makefile
└── .gitignore
```

---

# 🎓 Learning Objectives

By completing this project, the goal is to understand:

### Distributed Systems

* How distributed nodes communicate
* How data can be partitioned
* Why replication is necessary
* How failures affect distributed systems
* Why consistency is difficult
* How systems recover from failures

### Networking

* How TCP communication works
* How sockets work in C
* How application protocols are designed
* How concurrent network servers operate

### Systems Programming

* Memory management
* Processes
* Threads
* Synchronization
* File descriptors
* Low-level debugging

### Cybersecurity

* How memory vulnerabilities occur
* Why untrusted input is dangerous
* How crashes can reveal programming errors
* How to analyze vulnerable binaries
* How defensive mitigations work
* How to transform vulnerable code into safer code

---

# ⚠️ Responsible Use

This repository is intended for **education, research, and defensive security learning**.

The security laboratory must only be used against:

* Your own programs
* Your own virtual machines
* Local laboratory environments
* Systems where you have explicit authorization

Do not deploy intentionally vulnerable components on public infrastructure.

Do not use the techniques learned here against systems without authorization.

---

# 📚 Project Philosophy

The project follows one central idea:

> **Understand the system from the inside, including how it fails.**

A distributed system should not only work when everything goes correctly.

It should be studied under:

```text
Normal operation
      ↓
High concurrency
      ↓
Network failures
      ↓
Node failures
      ↓
Invalid input
      ↓
Unexpected memory conditions
      ↓
Security analysis
      ↓
Recovery and hardening
```

The objective is to understand both **how to build systems** and **how to build them safely**.

---

# 🚧 Project Status

Currently under development.

The project will be implemented incrementally, beginning with a single-node key-value store and progressively introducing networking, multiple nodes, replication, concurrency, failure handling, benchmarking, and security analysis.

---

## Author

**Anes**

Master 1 — Artificial Intelligence, Data and Agentic (AIDA)
Paris Dauphine-PSL

---

## License

This project is intended primarily for educational and research purposes.
