<p align="center"><img src="https://i.ibb.co/n0xQ1RY/logo-transparent.png" height="300"></p>
<p align="center">
  
  <img src="https://img.shields.io/badge/twitter-%40bitcoinglobalio-blue?style=flat-square&logo=twitter" alt="Twitter">
  <br>
  <img alt="GitHub tag (latest SemVer)" src="https://img.shields.io/github/v/tag/bitcoin-global/bitcoin-global?sort=semver">
 <!--  <a href="https://travis-ci.com/github/bitcoin-global/bitcoin-global/builds" target="_blank">
     <img src="https://travis-ci.com/bitcoin-global/bitcoin-global.svg?token=Kiztszp4vesa1iqMmSZ1&branch=master">
   </a>-->
  <img src="https://img.shields.io/badge/status-stable-green.svg">
  <a href="https://github.com/bitcoin-global/bitcoin-global/releases">
   <!-- <img alt="GitHub All Releases" src="https://img.shields.io/github/downloads/bitcoin-global/bitcoin-global/total"> 
  </a>
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/bitcoin-global/bitcoin-global"><br>-->
</p>



What is Bitcoin Global?
----------------

By introducing smaller blocks of 400KB, Bitcoin Global, a Bitcoin hardfork, implements a sustainable economic model for all chain participants, 
ensuring at the same time that decentralization and profitability are maintained. Most importantly, 
smaller blocks translate to a higher total number of nodes due to lesser computational power requirements 
and more nodes on the other side ensure an increased degree of decentralization.

For more information, as well as an immediately useable, binary version of
the Bitcoin Global software, see https://bitcoin-global.io or read the
[original documentation](https://bitcoin-global.io/assets/images/documentation.pdf).


Specifications
-------
Specification         | Value
---                   | ---
Supply                | 21.1 million
Premine               | 0.1 million
PoW algorithm         | SHA256
Mining hardware       | ASIC              
Block interval        | 10 minutes
Block size            | 0.4 MB
Difficulty adjustment | 10 minutes
SegWit                | :heavy_check_mark:
Replay protection     | :heavy_check_mark:

## Resources

* [Blockchain Explorer](https://mainnet.bitcoin-global.io/)
* [Documentation](https://bitcoin-global.io/assets/images/documentation.pdf)

### Community


* [Telegram](https://t.me/BitcoinGlobalOffical)
* [Discord](https://discord.gg/JFtQdk9)
* [Twitter](https://twitter.com/bitcoinglobalio)
* [Facebook](https://www.facebook.com/BitcoinGlobalGLOB)
* [Reddit](https://www.reddit.com/user/Bitcoin-Global)

License
-------

Bitcoin Global is released under the terms of the MIT license. See [COPYING](COPYING) for more
information.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/bitcoin-global/bitcoin-global/tags) are created
regularly to indicate new official, stable release versions of Bitcoin Global.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md)
and useful hints for developers can be found in [doc/developer-notes.md](doc/developer-notes.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

Travis CI and Circle CI systems make sure that every change is built, tested and released for Windows, Linux, and macOS, and that unit/sanity tests are run automatically.

