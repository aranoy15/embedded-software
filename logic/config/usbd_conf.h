#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <familyconf.h>

/*---------- -----------*/
#define USBD_MAX_NUM_INTERFACES     1
/*---------- -----------*/
#define USBD_MAX_NUM_CONFIGURATION     1
/*---------- -----------*/
#define USBD_MAX_STR_DESC_SIZ     512
/*---------- -----------*/
#define USBD_DEBUG_LEVEL     0
/*---------- -----------*/
#define USBD_SELF_POWERED     1
/*---------- -----------*/
#define MAX_STATIC_ALLOC_SIZE     512

/****************************************/
/* #define for FS and HS identification */
#define DEVICE_FS 		0

/**
  * @}
  */

/** @defgroup USBD_CONF_Exported_Macros USBD_CONF_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* Memory management macros */

/** Alias for memory allocation. */
#define USBD_malloc (uint32_t *)USBD_static_malloc

/** Alias for memory release. */
#define USBD_free  USBD_static_free

/** Alias for memory set. */
#define USBD_memset /* Not used */

/** Alias for memory copy. */
#define USBD_memcpy /* Not used */

void *USBD_static_malloc(uint32_t size);
void USBD_static_free(void *p);

/** Alias for delay. */

#if (USE_FREERTOS)
#define USBD_Delay vTaskDelay
#else
#define USBD_Delay HAL_Delay
#endif

#if (USBD_DEBUG_LEVEL > 0)
#define USBD_UsrLog(...)    printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_UsrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 1)

#define USBD_ErrLog(...)    printf("ERROR: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_ErrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 2)
#define USBD_DbgLog(...)    printf("DEBUG : ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_DbgLog(...)
#endif

#ifdef __cplusplus
}
#endif