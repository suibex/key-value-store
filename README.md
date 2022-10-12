# key-value-store
Basic key-value pair DB server in Python and ctypes.
<h1>How it works?</h1>
<br>This server is working on port 3322 , and is using AES encryption I implemented in C. (but uses python wrapper with ctypes).
<br>
<h1>Production?</h1>
hell nah, not ready for production, but it could be used , if really necessary.<br>
it accepts all connections on port 3322 that are checked with POSEIDON_HEADER (included in server.py)

