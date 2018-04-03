Minexcoin Core version 1.3.1 is now available from:

  <https://minexcoin.com/bin/>

This is a new minor version release, including various bugfixes and
performance improvements, as well as activated testnet.

Please report bugs using the issue tracker at github:

  <https://github.com/minexcoin/minexcoin/issues>

Compatibility
==============

Minexcoin Core is extensively tested on multiple operating systems using
the Linux kernel, macOS 10.8+, and Windows 7 and later.

Minexcoin Core should also work on most other Unix-like systems but is not
frequently tested on them.

Notable changes
===============

Testnet changes
---------------

- Testnet was activated.

These changes doesn't break any functionality of mainnet on past releases.

Reliability
-----------

- First checkpoint refers on block #110000 was added.

- Assume valid parameter's value and minimumchainwork's value were changed
  according to block #109800.

GUI
---

- Rewrited notification's widget for accepting version number of new format.

1.3.1 Change log
=================

Detailed release notes follow. This overview includes changes that affect
behavior, not code moves, refactors and string updates. For convenience in locating
the code changes and accompanying discussion, both the pull request and
git merge commit are mentioned.

### GUI
- #40 `4329a39` MC-41 Remake notification widget according to new version naming rules

### Testnet
- #41 `469ccf2` MC-39 Create testnet.

### Build and naming
- #39 `5695e38` Fix naming