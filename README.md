# SimplestDB
Summary: The goal of this project is to learn about the inner
workings of a relational database engine. 

Goal: Create a working RDBMS from scratch without using any external libraries/frameworks
other than the C++ Standard Library.

Currently supported SQL operations examples:

CREATE TABLE users (email VARCHAR, id INT, premium BOOLEAN); <br/> 
INSERT INTO users (VARCHAR, INT, BOOLEAN) VALUES (johnsmith@gmail.com, 10022, false); <br/>
SELECT email FROM users; <br/>


## Components
### Parser
Parses SQL queries and metacommands.

### Data Structure/File Formats
The main data structure used is a table directory. This allows for fast inserts. 
Query operations will require a table scan every time.

### Interpreter
Responsible for executing all operation tokens given by the parser.

### Disk Manager
Responsible for loading and writing data to the disk. 
LRU cache will be used to speed up operations.