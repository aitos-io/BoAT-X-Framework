# Contributing

We are glad to have contributors out of the core team; contributions, including (but not limited to) style/bug fixes,
implementation of features, proposals of schemes/algorithms, and thorough documentation, are welcomed. Feel free to follow us and fork the repository.

## Report bugs

You can report bugs to us through the [BoAT-X Issues](https://github.com/aitos-io/BoAT-X-Framework/issues). We appreciate a detailed description of the problem you met, including but not limited to the version, the device/chip brand and model, the blockchain etc. It is welcome to provide the detailed steps that could reproduce the bug. If applicable, log files or screenshots could also help.

## Contributor license agreement

All contributors and their contribution are required to agree with and follow [Apache License 2.0](./LICENSE).


## Submit your code

Before submitting your code, make sure to follow the [Contributor license agreement](#contributor-license-agreement) beforehand. DO NOT submit your code unless you fully accept the aggreement. Your submission implies you have accepted the agreement.

What you submit should link to an issue in the [BoAT-X Issues](https://github.com/aitos-io/BoAT-X-Framework/issues). Create one if there is no issue about what you're going to submit. Please also create a Pull Request for the issue and submission.


### Commit message format
The commit messages must be formatted. Refer to [AngularJS commit message format](https://docs.google.com/document/d/1QrDFcIiPjSLDn3EL15IJygNPiHORgU1_OOAqWjiDU5Y/edit#heading=h.4e0o8t4fffjf) for details.

Each commit message consists of a header, a body, and a footer.
```
<header>
<BLANK LINE>
<body>
<BLANK LINE>
<footer>
```
* #### header
  
  ```
  <type>(<scope>): <short summary>
  ```
  Header format is list as above, among them, 
  `type` is one of the following options:
     - **feat** (feature)
     - **fix** (bug fix)
     - **docs** (documentation)
     - **style** (formatting, missing semi colons, …)
     - **refactor** (code change that neither fixes a bug nor adds a feature)
     - **test** (when adding missing tests)
     - **chore** (maintain) 

    `scope` is optional.
    `short summary` is the short description of the commit.
* #### body
    `body` is the detail description of the commit, it's optional.
* #### footer
     `footer` is optional.

#### Some commit message samples
 - A submission with bug A fixed.
   - the simple commit message maybe:
     ```
     fix: fix bug A
     ```
     or
     ```
      fix: fix bug A

      the detail description of fixing bug A
     ```
   - If you want to emphasize the scope of the bug's impact(for example, the bug affects module X), the commit message may be:
     ```
      fix(module X): fix bug A
     ```
     or
     ```
      fix(module X): fix bug A

      the detail description of fixing bug A
     ```
   - If you want to close some issue in this commit(for example, issue #9 need to be closed), the commit message may be:
      ```
     fix(module X): fix bug A

     fix #9
     ```
     or
     ```
      fix(module X): fix bug A

      the detail description of fixing bug A

      fix #9
     ```
- A submission with feature A added.
  -  the simple commit message maybe:
     ```
     feat: add new feature A
     ```
     or
     ```
     feat: add feature B

     the detail description of adding feature B
     ```

#### Easy formatting tools
You could also use tools such as [git-cz](https://github.com/streamich/git-cz) to easily format the commit message.
