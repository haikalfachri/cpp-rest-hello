# Example RestAPI + ZeroMQ + SSE using C++

## Important notes

1. Install cmake via installer from https://cmake.org/download/ and add  ```/path/to/CMake/bin ``` to your path environment
   
2. Install vcpkg via clone, https://vcpkg.io/en/getting-started
   
3. Install ZeroMQ with vcpkg

    ```
    vcpkg install cppzmq
    ```
    
4. Install Rapid-JSON

    ```
    vcpkg install rapidjson
    ```

5. Install PQXX for Postgresql Database
    ```
    vcpkg install libpqxx
    ```

6. Install restbed for SSE server
    ```
    vcpkg install restbed
    ```
    
7. Adjust environment in .env

