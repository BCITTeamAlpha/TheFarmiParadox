#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OpenGLTest/EventManager.h"
#include "../OpenGLTest/EventManager.cpp"
#include <thread>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
    // An example class that implements ISubscriber and defines its own notify method
    class ExampleSubscriber : public ISubscriber {
    public:
        int storedData;

        ExampleSubscriber(int data) : storedData(data) {}

        void ExampleSubscriber::notify(EventName eventName, Param *param) {
            // Switch based on event type
            switch (eventName) {
            case EVENT_EXAMPLE: {

                // Safetly cast generic param pointer to a specific type
                TypeParam<int> *p = dynamic_cast<TypeParam<int> *>(param);

                if (p != nullptr) {
                    // Successful type cast
                    storedData = p->Param; // store param data
                } else {
                    // Incorrect type cast
                }

                break;
            }
            default:
                break;
            }
        }
    };

	TEST_CLASS(EventManagerTest)
	{
	public:

		TEST_METHOD(TestSync)
		{
            ExampleSubscriber ex(DEFAULT_DATA);
            Assert::AreEqual(ex.storedData, DEFAULT_DATA);
            
            EventManager::subscribe(EVENT_EXAMPLE, &ex);

            TypeParam<int> param(NEW_DATA);
            EventManager::notify(EVENT_EXAMPLE, &param, false);

            Assert::AreEqual(ex.storedData, NEW_DATA);
		}

        TEST_METHOD(TestAsync)
        {
            ExampleSubscriber ex(DEFAULT_DATA);
            Assert::AreEqual(ex.storedData, DEFAULT_DATA);

            EventManager::subscribe(EVENT_EXAMPLE, &ex);

            TypeParam<int> param(NEW_DATA);
            EventManager::notify(EVENT_EXAMPLE, &param);

            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            Assert::AreEqual(ex.storedData, NEW_DATA);
        }

        TEST_METHOD(TestUnsubscribe)
        {
            ExampleSubscriber ex(DEFAULT_DATA);
            Assert::AreEqual(ex.storedData, DEFAULT_DATA);

            EventManager::subscribe(EVENT_EXAMPLE, &ex);
            EventManager::unsubscribe(EVENT_EXAMPLE, &ex);

            TypeParam<int> param(NEW_DATA);
            EventManager::notify(EVENT_EXAMPLE, &param, false);

            Assert::AreEqual(ex.storedData, DEFAULT_DATA);
        }

    private:
        static const int DEFAULT_DATA = 5;
        static const int NEW_DATA = 8;
    };
}