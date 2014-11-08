
# 1 Business Scenario: Enterprise Archiving Solution for IT Risk Mitigation and Legal Compliance

The trends of electronic data growth naturally bring out new amendments to federal laws on data governance and compliance. Laws in the U.S and Canada from The Federal Rules of Civil Procedure (FRCP) and The Sedona Conference, respectively, outline models necessary for organizations to comply with the preservation of data integrity, as well as requiring reasonable steps to prevent data alteration or destruction. Amid litigation, parties must “define business records that are created or kept in electronic format as discoverable giving the requesting party access to them” as outlined in FRCP. This e-discovery requirement enshrines the principal that all relevant non-privileged electronic data must be disclosed to party claims or defenses given a request by the court.
	To comply with FRCP’s e-discovery requirements, all relevant business data should be archived to satisfy proper backup and recovery. A party’s discovery request is usually entitled to unaltered data, and this means the original files should be stored in its original formats with its metadata. Traditionally, this backup process utilized storage in magnetic tapes, disk drives across multiple machines, or a combination of both. However, with data growing at enormous rates and in numerous formats, the e-discovery process using outdated technology can become time consuming, expensive, and result in the exclusion of relevant data. In the case of data spoliation due to neglect, US courts have imposed sanctions in the form of monetary penalties and dismissals.

Our solution consists of a server component that processes enterprise data (emails, patents, files, etc.) and actively stores them to backup network devices. The server connects to data stores such as e-mail servers, file servers, or cloud services. Upon successful backup of a file, the server also indexes the data to a central repository and create metadata entries to a database. In the event of a legal discovery request, supported desktop applications or web portals can make search queries to the data repository through the indexed data. This enterprise wide system ensures that all data is backed up securely and effectively while featuring flexible and secure retrievals. In the event of e-discovery obligations, firms deployed with this system would have more control over their data to mitigate legal risks. 

## 2 Proxy Design Pattern

i) For the search and retrieval process of our system, we've chosen to use the *proxy design* pattern. 

ii) Due to the potential nature of extremely large file sizes and the sheer number of them in large organizations (possibly hundreds of millions), it is not prudent to transmit the original files across the network during e-discovery searches; transfer of large result sets would simply not be feasible due to limiting network resources and long processing times. To address this issue, we deploy a file proxy in place of the original data file that contains only the essential metadata required for search and retrieval. In essence, discovery clients would receive results in the form of small, memory efficient, file proxies that represent the original large documents. The retrieval of the original file would therefore be delayed until the file proxy has been explicitly opened. 

## 3 Graphical Representation in UML

[UML Diagram (Draw.io)](https://drive.google.com/file/d/0B110Jx0kEe5IQlYxajBycUZQR1U/view?usp=sharing)


## 4 How it works

A `DiscoveryClient` can be in the form of a desktop, web, or a mobile application. The client accesses the internal `SearchEngine` of our archive system by submitting search queries. Upon the initial search result, the `SearchEngine` returns -- over the network -- a set of `ItemProxy` items. Each `ItemProxy` merely represents the original file, like an alias, and contain only the essential metadata. If the `DiscoveryClient` were to open that `ItemProxy` explicitly from the list of results, `SearchEngine` delegates the request of opening the original file, i.e. calling the `openOriginal()` method, and `ItemProxy` will instantiate the appropriate file type for as needed.  

This proxy design solves the e-discovery problem of the need to access files remotely and manage system memory and network traffic appropriately; we implement `ItemProxy` as both a remote and virtual proxy. `ItemProxy` acts as a remote proxy in the sense that locally, clients can see files on a different address space while having full access to them. It acts as a virtual proxy in the sense that expensive objects (e.g. large files, or huge result sets) are created, or transferred over the network on demand. Some implications include: the ability for multiple users to access the central archive repository; and another, the ability to access the files virtually from any computer device without high cost of network usage and congestion.   

## 5 Skeleton C++ Code

ProxyPattern/src

## 6 Explanation of Code

The DiscoveryClient Class connects to a SearchEngine via `DiscoveryClient::connectToServer()`. Realistically, this would be implemented over the network. Once the SearchEngine has authenticated the client connection, the client can make queries using the call `searchQuery()`. The returned result set from the SearchEngine is in the form of `ItemProxy`s. This list of proxies hold no real data and is cached in the DiscoveryClient, represented by `ipResultsCache_`. `ipResultsCache_` is a set of proxy items that may represent generic system file types, images, or an enterprise mail. Here, the `vector<ItemProxy *>` container holds only the base class and by implementing a "is-a" relationship among the file types, we can use polymorphism to create derived classes using the same container. 

The DiscoveryClient can then display the result cache, using `displayResults()`,to its user. Again, this would only display the essential meta-data of each result such as file names, email recipients, or file abstracts, etc. In our example, the proxy items only contain two properties: `int id_` and `string desc_` to identify the original files. 

Once a user decides to explicitly open a particular file from the result set, the client can make a call to `openOriginal()`. This explicit call delegates the "open" call to the proxy item. Because the proxy item only represent a placeholder, it's responsible for retrieving the full contents of the original file. Due to differing location and retrieval methods of each file type, all the file types implement their own `openOriginal()` method. 


## 7 Execution Trace
```
DiscoveryClient:	Starting Discovery System
DiscoveryClient:	Initiating case # 234
DiscoveryClient:	Active case: Proxy PA Ltd vs. Observer PD Corp. 
SearchEngine:		Starting SearchEngine
DiscoveryClient:	Connected to SearchEngine
SearchEngine:		Processing query: All files regarding patent XY from 2000 to 2014
DiscoveryClient:	Successfully received results: 7
DiscoveryClient:	Displaying Proxy Results:
DiscoveryClient:	id: 0 System File
DiscoveryClient:	id: 1 Email File
DiscoveryClient:	id: 2 Image File
DiscoveryClient:	id: 3 System File
DiscoveryClient:	id: 4 Image File
DiscoveryClient:	id: 5 Email File
DiscoveryClient:	id: 6 System File
DiscoveryClient:	Requesting full restore of item 2
2:		Requesting original image file from proxy...
Opening original Image File: 2
DiscoveryClient:	Requesting full restore of item 3
3:		Requesting original system file from proxy...
Opening original System File: 3
```

## 8 Lessons Learnt
1. The proper use and implementation of the proxy pattern
2. Sometimes it's not sufficient for a system to just work, but we must also need to consider performance and resource management requirements 
3. We can use design patterns, such as the proxy pattern, to better abstract away specific details where not needed to simplify the system. In our scenario, the proxy item acted as an abstraction away from the resource intensive item. The search client, upon receiving the search results, was only concerned with the general, abstract, details in its result set. 





