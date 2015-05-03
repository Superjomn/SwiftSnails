.. doc documentation master file, created by
   sphinx-quickstart on Sun May  3 13:12:32 2015.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to SwiftSnails's documentation!
==========================================
SwiftSnails 是一个小型的分布式参数服务器。
适用于一些参数比较固定的模型的分布式计算，比如Logistic Regression，或者FM，神经网络这些模型。

提供参数的分布式存储，以及PUSH/PULL访问接口，用户可以在这些接口之上实现具体的逻辑，包括batch sgd或者minibatch sgd。

项目后续的发展方向是一些性能优化，和多一点的Demo。

欢迎有大神贡献代码，意见和建议。

任何一个项目都是为一个场景而服务，目前SwiftSnails实验的锅比较小，所以身量有限，如果应用的多的话，肯定会慢慢丰满起来。

工作方式
---------

.. figure:: ./_static/system.jpg
    :align: center
    :width: 700

    系统结构


目录
-----
作为框架，SwiftSnail包含一些接口，当然代码量也比较少，即使全部看完，也比较容易。

一些有用的目录如下：

* src : 项目源码
* src/core : 项目核心代码
* src/core/framework :框架模板（主要接口）
* src/core/utils: 框架的一些底层代码
* src/core/parameter: 有关参数的接口
* src/core/transfer: 消息传递的框架，用户基本用不到
* src/core/system: 参数服务器的内置逻辑，用户基本用不到
* src/apps: 一些demo的代码，目前有word2vec
* src/tools: 一些脚本工具，基本不能直接用，需要根据自己的环境修改
* src/unitest: 项目早期的一些单元测试


.. toctree::
   :maxdepth: 2



Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

