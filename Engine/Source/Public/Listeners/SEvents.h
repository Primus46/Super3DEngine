#pragma once
#include "EngineTypes.h"

// System Libs
#include <functional>

template<typename... Args>
class SEvents {
public:
	// adding a function into our callbacks array
	SUi8 Bind(const std::function<void(Args...)>& callback) {
		// make a unique structure or SCallback
		auto newNode = TMakeUnique<SCallbackNode>();
		// move the function into the unique struct
		newNode->callback = std::move(callback);
		// this will give us the index for the callback as push_back adds the elements to the end
		SUi8 id = 0;

		if (m_callbackNodes.size() > 0) {
			// find an id to assign to the callback node
			//starting with 1 id
			SUi8 potentialID = 1;

			// looping through each number to test if an id exists
			while (id == 0) {
				// defaults the found as false
				bool foundId = false;
				// if a node has that id set found to false and break out of the loop
				for(const auto& node : m_callbackNodes) {
					if (node->id == potentialID) {
						foundId = true;
						break;
					}
				}

				// if no id matches the potential id then breakl the while loop and set id
				if (!foundId) {
					id = potentialID;
					break;
				}
				
				// if an id was found that matched increase to the next potential
				++potentialID;
			}
		}
		else {
			id = 0;
		}
		

		newNode->id = id;

		// add the unique callback into the array
		m_callbackNodes.push_back(std::move(newNode));

		return id;
	}

	// run all functions bound to this event listeners
	void Run(const Args&... args){
		// loop through all of the stored functions
		for (const auto& node : m_callbackNodes) {
			// run each function with the arguments
			node->callback(args...);
		}
	}
	
	// unbind a function based on the the index 
	// get the index from the initial bind
	void Unbind(const SUi8& index) {
		std::erase_if(m_callbackNodes.begin(), m_callbackNodes.end(),
			[index](const SCallbackNode& node) {
				return node->id == index;
			}
		);
	}

private:
	struct SCallbackNode {
		std::function<void(Args...)> callback;
		SUi8 id;
	};

	// storing function to run when the event runs
	TArray<TUnique<SCallbackNode>> m_callbackNodes;

};

typedef SEvents<> SEventsVoid;