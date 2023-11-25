//============================================================================//
#include "DRV_SSR.h"

//------------------------------------------------------------------------------
extern "C" {
    //--------------------------------------------------------------------------
    //void __svc(0x01) SSR_IncludeComponent(uint32_t);
	#ifdef __GNUC__
		void __attribute__((naked)) __attribute__((noinline)) SSR_IncludeComponent(uint32_t) {
			__asm("svc %0" : : "I" (SVC_INCLUDE_COMPONENT));
			__asm("bx lr");
		}
    #else
        #pragma inline = never
  		__irq void SSR_IncludeComponent(uint32_t){
			__asm("svc %0" : : "I" (SVC_INCLUDE_COMPONENT));
			__asm("bx lr");
        }
	#endif

	//--------------------------------------------------------------------------
    //void __svc(0x02) SSR_ExcludeComponent(uint32_t);
	#ifdef __GNUC__
		void __attribute__((naked)) __attribute__((noinline)) SSR_ExcludeComponent(uint32_t) {
			__asm("svc %0" : : "I" (SVC_EXCLUDE_COMPONENT));
			__asm("bx lr");
		}
    #else
        #pragma inline = never
   		__irq void SSR_ExcludeComponent(uint32_t){
			__asm("svc %0" : : "I" (SVC_EXCLUDE_COMPONENT));
			__asm("bx lr");
        }

	#endif

	//--------------------------------------------------------------------------
    //uint32_t __svc(0x03) SSR_InstallCallback(uint32_t, uint32_t);
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wreturn-type"
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_InstallCallback(uint32_t, uint32_t) {
			__asm("svc %0" : : "I" (SVC_INSTALL_CALLBACK));
			__asm("bx lr");
		}
    #else
        #pragma diag_suppress=Pe940
        #pragma inline = never        
    	__irq uint32_t SSR_InstallCallback(uint32_t, uint32_t){
			__asm("svc %0" : : "I" (SVC_INSTALL_CALLBACK));
			__asm("bx lr");          
        }
	#endif

    //--------------------------------------------------------------------------        
    //void __svc(0x04) SSR_InstallCallback(uint32_t, uint32_t); NOT USED

	//--------------------------------------------------------------------------
	//uint32_t __svc(0x05) SSR_GetCallback(uint32_t);
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wreturn-type"
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_GetCallback(uint32_t) {
			__asm("svc %0" : : "I" (SVC_GET_CALLBACK));
			__asm("bx lr");
		}
    #else
        #pragma diag_suppress=Pe940
        #pragma inline = never        
		__irq uint32_t SSR_GetCallback(uint32_t){
			__asm("svc %0" : : "I" (SVC_GET_CALLBACK));
			__asm("bx lr");
        }
        #pragma diag_warning=Pe940
	#endif

	//--------------------------------------------------------------------------
    //uint32_t __svc(0x06) SSR_GetSystemTime();
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wreturn-type"
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_GetSystemTime(void) {
			__asm("svc %0" : : "I" (SVC_GET_SYSTEM_TIME));
			__asm("bx lr");
		}
    #else
        #pragma diag_suppress=Pe940
        #pragma inline = never
    	__irq uint32_t SSR_GetSystemTime(void) {
			__asm("svc %0" : : "I" (SVC_GET_SYSTEM_TIME));
			__asm("bx lr");
        }
        #pragma diag_warning=Pe940
	#endif

	//--------------------------------------------------------------------------
    //bool __svc(0x07) SSR_InstallTimeout(uint32_t, uint32_t);
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wreturn-type"
		bool __attribute__((naked)) __attribute__((noinline)) SSR_InstallTimeout(uint32_t, uint32_t) {
			__asm("svc %0" : : "I" (SVC_INSTALL_TIMEOUT));
			__asm("bx lr");
		}
    #else
        #pragma diag_suppress=Pe940
        #pragma inline = never
        __irq bool SSR_InstallTimeout(uint32_t, uint32_t){
			__asm("svc %0" : : "I" (SVC_INSTALL_TIMEOUT));
			__asm("bx lr");
        }
        #pragma diag_warning=Pe940
	#endif

	//--------------------------------------------------------------------------
    //uint32_t __svc(0x08) SSR_GetKSCode();
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wreturn-type"
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_GetKSCode(void) {
			__asm("svc %0" : : "I" (SVC_GET_KS_CODE));
			__asm("bx lr");
		}
    #else
        #pragma diag_suppress=Pe940
        #pragma inline = never
    	__irq uint32_t SSR_GetKSCode(void){
			__asm("svc %0" : : "I" (SVC_GET_KS_CODE));
			__asm("bx lr");        
        }
        #pragma diag_warning=Pe940
	#endif

	//--------------------------------------------------------------------------
	//uint32_t __svc(0x09) SSR_GetCallbackVector(uint32_t);
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wreturn-type"
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_GetCallbackVector(uint32_t) {
			__asm("svc %0" : : "I" (SVC_GET_CALLBACK_VECTOR));
			__asm("bx lr");
		}
    #else
        #pragma diag_suppress=Pe940
        #pragma inline = never
        __irq uint32_t SSR_GetCallbackVector(uint32_t){
			__asm("svc %0" : : "I" (SVC_GET_CALLBACK_VECTOR));
			__asm("bx lr");
        }
        #pragma diag_warning=Pe940
	#endif

	//--------------------------------------------------------------------------
    //void __svc(0xFE) SSR_ThrowMessage(uint32_t, uint32_t, uint32_t, uint32_t);
	#ifdef __GNUC__
		void __attribute__((naked)) __attribute__((noinline)) SSR_ThrowMessage(uint32_t, uint32_t, uint32_t, uint32_t) {
			__asm("svc %0" : : "I" (SVC_THROW_MESSAGE));
			__asm("bx lr");
		}
    #else
        #pragma inline = never
        __irq void SSR_ThrowMessage(uint32_t, uint32_t, uint32_t, uint32_t){
			__asm("svc %0" : : "I" (SVC_THROW_MESSAGE));
			__asm("bx lr");
        }
	#endif

	//--------------------------------------------------------------------------
    //void __svc(0xFF) SSR_ThrowException(uint32_t);
	#ifdef __GNUC__
		void __attribute__((naked)) __attribute__((noinline)) SSR_ThrowException(uint32_t){
			__asm("svc %0" : : "I" (SVC_THROW_EXCEPTION));
			__asm("bx lr");
		}
    #else
    	__irq void SSR_ThrowException(uint32_t){
			__asm("svc %0" : : "I" (SVC_THROW_EXCEPTION));
			__asm("bx lr");
        }
	#endif

	//--------------------------------------------------------------------------
	//uint32_t __svc(SVC_MICROSECONDS) SSR_Microseconds(uint32_t);
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wreturn-type"
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_Microseconds(){
			__asm("svc %0" : : "I" (SVC_MICROSECONDS));
			__asm("bx lr");
		}
	#else
		#pragma diag_suppress=Pe940
		#pragma inline = never
		__irq uint32_t SSR_Microseconds(){
			__asm("svc %0" : : "I" (SVC_MICROSECONDS));
			__asm("bx lr");
		}
		#pragma diag_warning=Pe940
	#endif

		//--------------------------------------------------------------------------
		//uint32_t __svc(SVC_DELAY) SSR_Delay(uint32_t);
		#ifdef __GNUC__
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wreturn-type"
			uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_Delay(uint32_t){
				__asm("svc %0" : : "I" (SVC_DELAY));
				__asm("bx lr");
			}
		#else
			#pragma diag_suppress=Pe940
			#pragma inline = never
			__irq uint32_t SSR_Delay(uint32_t){
				__asm("svc %0" : : "I" (SVC_DELAY));
				__asm("bx lr");
			}
			#pragma diag_warning=Pe940
		#endif

			//--------------------------------------------------------------------------
			//uint32_t __svc(SVC_MICRODELAY) SSR_MicroDelay(uint32_t);
			#ifdef __GNUC__
				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wreturn-type"
				uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_MicroDelay(uint32_t){
					__asm("svc %0" : : "I" (SVC_MICRODELAY));
					__asm("bx lr");
				}
			#else
				#pragma diag_suppress=Pe940
				#pragma inline = never
				__irq uint32_t SSR_MicroDelay(uint32_t){
					__asm("svc %0" : : "I" (SVC_MICRODELAY));
					__asm("bx lr");
				}
				#pragma diag_warning=Pe940
			#endif


}

//------------------------------------------------------------------------------
extern "C" {
void SSR_Relocate(int nVectors){
	#define HW32_REG(ADDRESS) (*((volatile unsigned long *)(ADDRESS)))
	#define VTOR_NEW_ADDR 0x20000000
	int i;
	for (i=0; i<nVectors; i++){
		HW32_REG((VTOR_NEW_ADDR + (i<<2))) = HW32_REG((i<<2));
	}
	__DMB();
	SCB->VTOR = VTOR_NEW_ADDR;
	__DSB();
}}

//------------------------------------------------------------------------------
extern "C" {
void SSR_Allocate(uint32_t IsrAddress, uint32_t VectorIndex){
	#define HW32_REG(ADDRESS) (*((volatile unsigned long *)(ADDRESS)))
	#define VTOR_NEW_ADDR 0x20000000
	HW32_REG((VTOR_NEW_ADDR + (VectorIndex<<2))) = IsrAddress;
	__DMB();
	__DSB();
}}

//==============================================================================

