
### Delta sharing client for C++

Delta Sharing client for C++ provides a simple C++ based API for accessing Delta Shares natively in C++.

To build it (Mac OS):
Install apache-arrow, and nlohmann-json using homebrew:

```sh
brew install cmake
brew install boost
brew install openSSL
brew install apache-arrow
brew install nlohmann-json
brew install automake

# sometimes has CMAKE a hard time finding packages
# by setting these you should help the arrow find package quite much
export PARQUET_HOME=/usr/local/opt/apache-arrow
export ARROW_HOME=/usr/local/opt/apache-arrow

cd build
cmake .
make
make test
make install
```



The API is very straightforward
```cpp
functions.h
// Convenience functions for creating a delta sharing client

// <profile filepath>#<share>.<schema>.<table> to vector of strings with path,share,schema,table
const std::vector<std::string> ParseURL(std::string path); 

// Factory function to create a new DeltaSharingClient 
std::shared_ptr<DeltaSharingClient> NewDeltaSharingClient(std::string profile, boost::optional<std::string> cacheLocation);


// Convenience function to quickly get an Arrow Table from a path in the form:
// <profile filepath>#<share>.<schema>.<table>
// fileno == file number to load
const std::shared_ptr<arrow::Table> LoadAsArrowTable(std::string path, int fileno);

```


The DeltaSharing client exposes a number of methods allowing you to interact with the Delta Sharing server:


* ListShares
* ListSchemas
* ListTables
* ListAllTables
* ListFilesInTable
* QueryTableMetadata





