/**
 ****************************************************************************************************
 * @file        atim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.2
 * @date        2020-04-21
 * @brief       �߼���ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200421
 * ��һ�η���
 * V1.1 20200421
 * 1, ����atim_timx_comp_pwm_init����, ʵ������Ƚ�ģʽPWM�������
 * V1.2 20200421
 * 1, ����atim_timx_cplm_pwm_init����
 * 2, ����atim_timx_cplm_pwm_set����
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/atim.h"
#include "./BSP/LED/led.h"

/******************************************************************************************/
/* ��ʱ�����þ�� ���� */

/*�߼���ʱ��PWM*/
//static TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* ��ʱ��x��� */
//static TIM_OC_InitTypeDef g_timx_oc_pwm_chy_handle; /* ��ʱ�������� */

/* g_npwm_remain��ʾ��ǰ��ʣ�¶��ٸ�����Ҫ����
 * ÿ����෢��256������
 */
//static uint32_t g_npwm_remain = 0;

/*�߼���ʱ������Ƚ�ģʽ*/
static TIM_HandleTypeDef g_timx_comp_pwm_handle;                              /* ��ʱ��x��� */

/*�����������������*/
static TIM_HandleTypeDef g_timx_cplm_pwm_handle;                              /* ��ʱ��x��� */
static TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};             /* ����ʱ������ */

/* PWM����״̬(g_timxchy_cap_sta)
 * 0,û�гɹ�����.
 * 1,�Ѿ��ɹ�������
 */
uint8_t g_timxchy_pwmin_sta  = 0;   /* PWM����״̬ */
uint16_t g_timxchy_pwmin_psc  = 0;  /* PWM�����Ƶϵ�� */
uint32_t g_timxchy_pwmin_hval = 0;  /* PWM�ĸߵ�ƽ���� */
uint32_t g_timxchy_pwmin_cval = 0;  /* PWM�����ڿ�� */
static TIM_HandleTypeDef g_timx_pwmin_chy_handle;                              /* ��ʱ��x��� */


/******************************************************************************************/
/* HAL ͨ�ûص��ӿں��� */

///**
// * @brief       �߼���ʱ��TIMX NPWM�жϷ�����
// * @param       ��
// * @retval      ��
// */
//void ATIM_TIMX_NPWM_IRQHandler(void)
//{
//    HAL_TIM_IRQHandler(&g_timx_pwm_chy_handle); /* ��ʱ�����ô����� */
//}

///**
// * @brief       ��ʱ�������жϻص�����
// * @param        htim:��ʱ�����ָ��
// * @note        �˺����ᱻ��ʱ���жϺ�����ͬ���õ�
// * @retval      ��
// */
////void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
////{
////    uint16_t npwm = 0;

////    if (htim == (&g_timx_pwm_chy_handle)) /* �߼���ʱ���ж�ʵ��ص�ִ�е����� */
////    {
////        if (g_npwm_remain > 256) /* ���д���256��������Ҫ���� */
////        {
////            g_npwm_remain = g_npwm_remain - 256;
////            npwm = 256;
////        }
////        else if (g_npwm_remain % 256) /* ����λ��������256��������Ҫ���� */
////        {
////            npwm = g_npwm_remain % 256;
////            g_npwm_remain = 0; /* û�������� */
////        }

////        if (npwm) /* ������Ҫ���� */
////        {
////            g_timx_pwm_chy_handle.Instance->RCR = npwm - 1;                        /* �����ظ������Ĵ���ֵΪnpwm-1, ��npwm������ */
////            HAL_TIM_GenerateEvent(&g_timx_pwm_chy_handle, TIM_EVENTSOURCE_UPDATE); /* ����һ�θ����¼�,���ж����洦��������� */
////            __HAL_TIM_ENABLE(&g_timx_pwm_chy_handle);                              /* ʹ�ܶ�ʱ��TIMX */
////        }
////        else
////        {
////            g_timx_pwm_chy_handle.Instance->CR1 &= ~(1 << 0); /* �رն�ʱ��TIMX��ʹ��HAL Disable�����PWMͨ����Ϣ���˴����� */
////        }
////    }
////}

///**
// * @brief       �߼���ʱ��TIMX ͨ��Y ���ָ������PWM ��ʼ������
// * @note
// *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 72Mhz, ��������PPRE2����Ƶ, ���
// *              �߼���ʱ��ʱ�� = 72Mhz
// *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
// *              Ft=��ʱ������Ƶ��,��λ:Mhz
// *
// * @param       arr: �Զ���װֵ
// * @param       psc: ʱ��Ԥ��Ƶ��
// * @retval      ��
// */
//void atim_timx_npwm_chy_init(uint16_t arr, uint16_t psc)
//{
//    ATIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE(); /* TIMX ͨ��IO��ʱ��ʹ�� */
//    ATIM_TIMX_NPWM_CHY_CLK_ENABLE();      /* TIMX ʱ��ʹ�� */

//    g_timx_pwm_chy_handle.Instance = ATIM_TIMX_NPWM;                   /* ��ʱ��x */
//    g_timx_pwm_chy_handle.Init.Prescaler = psc;                        /* ��ʱ����Ƶ */
//    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_DOWN;     /* ���ϼ���ģʽ */
//    g_timx_pwm_chy_handle.Init.Period = arr;                           /* �Զ���װ��ֵ */
//    g_timx_pwm_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; /* ʱ�ӷ�Ƶ���� */
//    g_timx_pwm_chy_handle.Init.RepetitionCounter = 0;                  /* ��ʼʱ������ */
//    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);                          /* ��ʼ��PWM */

//    g_timx_oc_pwm_chy_handle.OCMode = TIM_OCMODE_PWM1;                                                /* ģʽѡ��PWM1 */
//    g_timx_oc_pwm_chy_handle.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* ����Ƚϼ���Ϊ�� */
//    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &g_timx_oc_pwm_chy_handle, ATIM_TIMX_NPWM_CHY); /* ����TIMxͨ��y */

//    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, ATIM_TIMX_NPWM_CHY);      /* ������ӦPWMͨ�� */
//    __HAL_TIM_ENABLE_IT(&g_timx_pwm_chy_handle, TIM_IT_UPDATE);         /* ʹ�ܸ����ж� */

//    HAL_NVIC_SetPriority(ATIM_TIMX_NPWM_IRQn, 1, 3);                    /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
//    HAL_NVIC_EnableIRQ(ATIM_TIMX_NPWM_IRQn);                            /* ����ITMx�ж� */
//}

///**
// * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
//                �˺����ᱻHAL_TIM_PWM_Init()����
// * @param       htim:��ʱ�����
// * @retval      ��
// */
////void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
////{
////    if (htim->Instance == ATIM_TIMX_NPWM)
////    {
////        GPIO_InitTypeDef gpio_init_struct;
////        ATIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE(); /* ����ͨ��y��CPIOʱ�� */

////        gpio_init_struct.Pin = ATIM_TIMX_NPWM_CHY_GPIO_PIN; /* ͨ��y��CPIO�� */
////        gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ����������� */
////        gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
////        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* ���� */
////        HAL_GPIO_Init(ATIM_TIMX_NPWM_CHY_GPIO_PORT, &gpio_init_struct);
////    }
////}

///**
// * @brief       �߼���ʱ��TIMX NPWM����PWM����
// * @param       rcr: PWM�ĸ���, 1~2^32�η���
// * @retval      ��
// */
//void atim_timx_npwm_chy_set(uint32_t npwm)
//{
//    if (npwm == 0)
//        return;

//    g_npwm_remain = npwm;                                                  /* ����������� */
//    HAL_TIM_GenerateEvent(&g_timx_pwm_chy_handle, TIM_EVENTSOURCE_UPDATE); /* ����һ�θ����¼�,���ж����洦��������� */
//    __HAL_TIM_ENABLE(&g_timx_pwm_chy_handle);                              /* ʹ�ܶ�ʱ��TIMX */
//}

/**
 * @brief       �߼���ʱ��TIMX ����Ƚ�ģʽ ��ʼ��������ʹ������Ƚ�ģʽ��
 * @note
 *              ���ø߼���ʱ��TIMX 4·����Ƚ�ģʽPWM���,ʵ��50%ռ�ձ�,��ͬ��λ����
 *              ע��,����������Ƚ�ģʽ,ÿ2���������ڲ������һ��PWM���,������Ƶ�ʼ���
 *              ����,���ǻ����Կ����ж����ж������޸�CCRx,�Ӷ�ʵ�ֲ�ͬƵ��/��ͬ��λ�Ŀ���
 *              �������ǲ��Ƽ���ôʹ��,��Ϊ����ܵ��·ǳ�Ƶ�����ж�,�Ӷ�ռ�ô���CPU��Դ
 *
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 72Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void atim_timx_comp_pwm_init(uint16_t arr, uint16_t psc)
{

    ATIM_TIMX_COMP_CLK_ENABLE();            /* ʹ�ܶ�ʱ��ʱ�� */
    
    TIM_OC_InitTypeDef sConfigOC = {0};

    g_timx_comp_pwm_handle.Instance = ATIM_TIMX_COMP;                       /* ��ʱ��8 */
    g_timx_comp_pwm_handle.Init.Prescaler = psc  ;                          /* ��ʱ����Ƶ */
    g_timx_comp_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;           /* ���ϼ���ģʽ */
    g_timx_comp_pwm_handle.Init.Period = arr;                               /* �Զ���װ��ֵ */
    g_timx_comp_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;     /* ʱ�ӷ�Ƶ���� */
    g_timx_comp_pwm_handle.Init.RepetitionCounter = 0;                      /* �ظ��������Ĵ���Ϊ0 */
    g_timx_comp_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_OC_Init(&g_timx_comp_pwm_handle);                               /*����Ƚϳ�ʼ��TIMx */

    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;         /* �Ƚ����ģʽ */
    sConfigOC.Pulse = 250;                        /* ��������ȽϼĴ�����ֵ */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;   /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &sConfigOC, TIM_CHANNEL_1);  /* ��ʼ����ʱ��������Ƚ�ͨ��1 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);           /* ͨ��1 Ԥװ��ʹ�� */

    sConfigOC.Pulse = 500;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &sConfigOC, TIM_CHANNEL_2);  /* ��ʼ����ʱ��������Ƚ�ͨ��2 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);           /* ͨ��2 Ԥװ��ʹ�� */

    sConfigOC.Pulse = 750;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &sConfigOC, TIM_CHANNEL_3);  /* ��ʼ����ʱ��������Ƚ�ͨ��3 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);           /* ͨ��3 Ԥװ��ʹ�� */

    sConfigOC.Pulse = 1000;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &sConfigOC, TIM_CHANNEL_4);  /* ��ʼ����ʱ��������Ƚ�ͨ��4 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);           /* ͨ��4 Ԥװ��ʹ�� */
    
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle,TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle,TIM_CHANNEL_2);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle,TIM_CHANNEL_3);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle,TIM_CHANNEL_4);
}

/**
 * @brief       ��ʱ������Ƚϵײ�����
                HAL����õĽӿڣ��������ò�ͬ�����벶��
 * @param       htim:��ʱ�����
 * @note        �˺����ᱻHAL_TIM_OC_Init()����
 * @retval      ��
 */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef* htim)
{

    if (htim->Instance == ATIM_TIMX_COMP)
    {
        GPIO_InitTypeDef gpio_init_struct = {0};
        
        ATIM_TIMX_COMP_CH1_GPIO_CLK_ENABLE();   /* ͨ��1��ӦIO��ʱ��ʹ�� */
        ATIM_TIMX_COMP_CH2_GPIO_CLK_ENABLE();   /* ͨ��2��ӦIO��ʱ��ʹ�� */
        ATIM_TIMX_COMP_CH3_GPIO_CLK_ENABLE();   /* ͨ��3��ӦIO��ʱ��ʹ�� */
        ATIM_TIMX_COMP_CH4_GPIO_CLK_ENABLE();   /* ͨ��4��ӦIO��ʱ��ʹ�� */

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH1_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH1_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH2_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH2_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH3_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH3_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH4_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH4_GPIO_PORT, &gpio_init_struct);
    }
}

/*******************************����������������Ƴ���**************************************/

/**
 * @brief       �߼���ʱ��TIMX ������� ��ʼ��������ʹ��PWMģʽ1��
 * @note
 *              ���ø߼���ʱ��TIMX �������, һ·OCy һ·OCyN, ���ҿ�����������ʱ��
 *
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 72Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */

void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef sConfigOC = {0};
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    ATIM_TIMX_CPLM_CLK_ENABLE();            /* TIM1 ʱ��ʹ�� */
    ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE();   /* ͨ��X��ӦIO��ʱ��ʹ�� */
    ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE();  /* ͨ��X����ͨ����ӦIO��ʱ��ʹ�� */
    ATIM_TIMX_CPLM_BKIN_GPIO_CLK_ENABLE();  /* ͨ��Xɲ�������ӦIO��ʱ��ʹ�� */
  
    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP; 
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHY_GPIO_PORT, &gpio_init_struct);
  
    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHYN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);
  
    gpio_init_struct.Pin = ATIM_TIMX_CPLM_BKIN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_BKIN_GPIO_PORT, &gpio_init_struct);

    g_timx_cplm_pwm_handle.Instance = ATIM_TIMX_CPLM;
    g_timx_cplm_pwm_handle.Init.Prescaler = psc;
    g_timx_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timx_cplm_pwm_handle.Init.Period = arr;
    g_timx_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    g_timx_cplm_pwm_handle.Init.RepetitionCounter = 0;
    g_timx_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&g_timx_cplm_pwm_handle) ;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;           /* OCy �͵�ƽ��Ч */
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         /* OCyN �͵�ƽ��Ч */
    HAL_TIM_PWM_ConfigChannel(&g_timx_cplm_pwm_handle, &sConfigOC, ATIM_TIMX_CPLM_CHY);     /*���ú�Ĭ����CCER�Ļ������λ*/
     
    /* ����PWM�������� */
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;              /* ����ģʽ�Ĺر����״̬ */
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;             /* ����ģʽ�Ĺر����״̬ */
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;                   /* ʹ��ɲ������ */
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;                   /* �رռĴ������� */
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;          /* ��·���뼫��Ϊ�� */
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;    /* �Զ��ָ����ʹ�ܿ��� */
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &sBreakDeadTimeConfig);  /* ��ʼ����ʱ���Ķ�·������ʱ�� */
    
    HAL_TIM_PWM_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);     /* OCy ���ʹ�� */
    HAL_TIMEx_PWMN_Start(&g_timx_cplm_pwm_handle,ATIM_TIMX_CPLM_CHY);   /* OCyN ���ʹ�� */

}

/**
 * @brief       ��ʱ��TIMX ��������Ƚ�ֵ & ����ʱ��
 * @param       ccr: ����Ƚ�ֵ
 * @param       dtg: ����ʱ��
 *   @arg       dtg[7:5]=0xxʱ, ����ʱ�� = dtg[7:0] * tDTS
 *   @arg       dtg[7:5]=10xʱ, ����ʱ�� = (64 + dtg[6:0]) * 2  * tDTS
 *   @arg       dtg[7:5]=110ʱ, ����ʱ�� = (32 + dtg[5:0]) * 8  * tDTS
 *   @arg       dtg[7:5]=111ʱ, ����ʱ�� = (32 + dtg[5:0]) * 16 * tDTS
 *   @note      tDTS = 1 / (Ft /  CKD[1:0]) = 1 / 18M = 55.56ns
 * @retval      ��
 */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg)
{
    sBreakDeadTimeConfig.DeadTime = dtg;
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &sBreakDeadTimeConfig);      /*��������ʱ��*/
    __HAL_TIM_MOE_ENABLE(&g_timx_cplm_pwm_handle);  /* MOE=1,ʹ������� */    
    ATIM_TIMX_CPLM_CHY_CCRY = ccr;          /* ���ñȽϼĴ��� */
}

/*******************************�߼���ʱ��PWM����ģʽ����**************************************/

/**
 * @brief       ��ʱ��TIMX ͨ��Y PWM����ģʽ ��ʼ������
 * @note
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 72Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 *              ��������ʼ����ʱ��: ʹ��psc=0, arr�̶�Ϊ65535. �õ�����ʱ��Ƶ��Ϊ72Mhz,����Լ13.8ns
 *
 * @param       ��
 * @retval      ��
 */
void atim_timx_pwmin_chy_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};
    
    ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE();  /* TIMX ͨ��IO��ʱ��ʹ�� */
    ATIM_TIMX_PWMIN_CHY_CLK_ENABLE();       /* TIMX ʱ��ʹ�� */

    gpio_init_struct.Pin = ATIM_TIMX_PWMIN_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP; 
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    HAL_GPIO_Init(ATIM_TIMX_PWMIN_CHY_GPIO_PORT, &gpio_init_struct);

    g_timx_pwmin_chy_handle.Instance = ATIM_TIMX_PWMIN;     /* ��ʱ��x */
    g_timx_pwmin_chy_handle.Init.Prescaler = 0;             /* ��ʱ��Ԥ��Ƶϵ�� */
    g_timx_pwmin_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;  /* ���ϼ���ģʽ */
    g_timx_pwmin_chy_handle.Init.Period = 65535;            /* �Զ���װ��ֵ */
    g_timx_pwmin_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    /* ��Ƶ */
    g_timx_pwmin_chy_handle.Init.RepetitionCounter = 0;     /* �ظ��������Ĵ���Ϊ0 */
    g_timx_pwmin_chy_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;     /*ʹ��Ӱ�ӼĴ���TIMx_ARR*/
    HAL_TIM_IC_Init(&g_timx_pwmin_chy_handle);

    /* ��ģʽ���ã�IT1�������� */
    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;   /* ��ģʽ����λģʽ */
    sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;      /* ��ʱ�����봥��Դ��TI1FP1 */
    sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;     /*�����ؼ��*/
    sSlaveConfig.TriggerFilter = 0;                 /* ���˲� */
    HAL_TIM_SlaveConfigSynchro(&g_timx_pwmin_chy_handle,&sSlaveConfig);
    
    /* IC1���������ش���TI1FP1 */
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING; /* �����ؼ�� */
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;       /* ѡ�������IC1ӳ�䵽TI1 */
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;                 /* ����Ƶ */
    sConfigIC.ICFilter = 0;                                 /* ���˲� */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle, &sConfigIC, TIM_CHANNEL_1);
    
    /* IC2���������ش���TI1FP2 */
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;    /* �½��ؼ�� */
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;         /*ѡ�������IC2ӳ�䵽TI1*/
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle, &sConfigIC, TIM_CHANNEL_2);
    
    HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_IRQn,1,3);           /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(ATIM_TIMX_PWMIN_IRQn);                 /* ����ITMx�ж� */

    /* TIM1/TIM8 �ж��������벶���жϷ����� */
    if (ATIM_TIMX_PWMIN == TIM1 || ATIM_TIMX_PWMIN == TIM8)
    {
        HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_CC_IRQn,1,3);    /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
        HAL_NVIC_EnableIRQ(ATIM_TIMX_PWMIN_CC_IRQn);          /* ����ITMx�ж� */
    }
    
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle,TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle,TIM_CHANNEL_2);
}

/**
 * @brief       ��ʱ��TIMX ����/��� �жϷ�����
 *   @note      TIM1/TIM8��������������ڸ���/����жϷ���,����������һ������!
 *              ������ͨ��ʱ�������/���/����,��������������洦��!
 * @param       ��
 * @retval      ��
 */
void ATIM_TIMX_PWMIN_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_pwmin_chy_handle);/* ��ʱ�����ô����� */
}

/**
 * @brief       ��ʱ��TIMX ���벶�� �жϷ�����
 *   @note      ��TIM1/TIM8���������,������ͨ��ʱ��û������жϷ�����!
 * @param       ��
 * @retval      ��
 */
void ATIM_TIMX_PWMIN_CC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_pwmin_chy_handle);/* ��ʱ�����ô����� */
}

/**
 * @brief       ��ʱ��TIMX PWM����ģʽ ������������
 * @param       ��
 * @retval      ��
 */
void atim_timx_pwmin_chy_restart(void)
{
    sys_intx_disable();                     /* �ر��ж� */

    g_timxchy_pwmin_sta = 0;                /* ����״̬,���¿�ʼ��� */
    g_timxchy_pwmin_hval=0;
    g_timxchy_pwmin_cval=0;

    sys_intx_enable();                      /* ���ж� */
}

/**
 * @brief       ��ʱ�����벶���жϴ���ص�����
 * @param       htim:��ʱ�����ָ��
 * @note        �ú�����HAL_TIM_IRQHandler�лᱻ����
 *              �˺����Ƕ�ʱ����ͬ���õĻص�������Ϊ�˷�ֹ�ض������
 *              ���ǰ�gtim.c��HAL_TIM_IC_CaptureCallback()���������ε�.
 * @retval      ��
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    
    if (g_timxchy_pwmin_sta == 0)   /* ��û�гɹ����� */
    {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {  
            g_timxchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle,TIM_CHANNEL_2)+2; /* ����ϵ��Ϊ2, ��1 */
            g_timxchy_pwmin_cval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle,TIM_CHANNEL_1)+2; /* ����ϵ��Ϊ2, ��1 */
            g_timxchy_pwmin_sta = 1;        /* ��ǲ���ɹ� */
        }
    }
}


