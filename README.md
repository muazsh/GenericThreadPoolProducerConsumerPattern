[![MSBuild](https://github.com/muazsh/GenericThreadPoolProducerConsumerPattern/actions/workflows/msbuild.yml/badge.svg)](https://github.com/muazsh/GenericThreadPoolProducerConsumerPattern/actions/workflows/msbuild.yml)

# Generic Thread Pool Producer/Consumer Pattern

Producer/Consumer is one of the standard concurrency problems and the most addressed one, and when such a problem pops up thread pool is of concern also, as resources are limited and it makes no sense to start a new thread whenever a new task (producer or consumer) comes.

This project introduces a generic pattern of Producer/Consumer problem provieded by a thread pool, so the programmer is only supposed to write the producing and consuming logics with some running conditions in order to create the pool and to start whatever producers and consumers needed.
