#pragma once
#ifndef ITASK
#define ITASK

namespace gtppcp {
	class ITask {
	public:
		virtual ~ITask() = default;
		virtual void Start() = 0;
	};
}

#endif // !ITASK
