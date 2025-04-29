
#ifndef __APP_SCREEN_COMMON_H
#define __APP_SCREEN_COMMON_H
typedef enum
{
    PAGE_TYPE_CONFIRM,       // ��Ҫȷ�ϵĽ��棬����ѡ��ȡ����ȷ��
    PAGE_TYPE_TIMER,         // ���ڴ���Ľ��棬����ѡ��ȡ��
    PAGE_TYPE_AUTO_FORWARD,  // ��ʱ��ҳ
    PAGE_TYPE_MESSAGE,       // ��Ϣ��ʾ
    PAGE_TYPE_MENU,          // �˵�
    PAGE_TYPE_ALERT,         // ��ʾ
    PAGE_TYPE_PAGED_MESSAGE, // �Զ���ҳ����Ϣ��ʾ
} app_screen_page_type_t;

typedef struct app_screen_page *app_screen_page_s_pt;
typedef void (*app_screen_handler)(app_screen_page_s_pt page);
typedef int (*app_screen_check_handler)(app_screen_page_s_pt page);
typedef void (*app_screen_before_handler)(app_screen_page_s_pt page, app_screen_page_s_pt parent);
typedef void (*app_screen_key_handler)(app_screen_page_s_pt page, uint8_t key);
typedef struct app_screen_page
{
    app_screen_page_type_t type;
    uint8_t initialized;      // �Ƿ��Ѿ���ʼ��
    uint8_t user_initialized; // �Ƿ��Ѿ��û���ʼ��
    uint8_t save_history;     // �Ƿ񱣴浽��ʷҳ��
    void *params;
    void *content;
    // struct app_screen_page *parent;     // ��һҳ
    app_screen_handler init;            //�����ʼ������
    app_screen_handler user_init;       //�����ʼ������--�û�ʹ��
    app_screen_check_handler check;     //init��beforeǰ���ã�������سɹ�����ʾ��ҳ�������˵���ҳ
    app_screen_before_handler before;   //��ʾ�����ǰ����--�û�ʹ��
    app_screen_handler before_render;   //��ʾ�����ǰ����--���ʹ��
    app_screen_handler render;          //�������Ⱦ����
    app_screen_handler after;           //�뿪���ʱ����--�û�ʹ��
    app_screen_key_handler key_handler; //����İ����¼�������
    app_screen_handler timer_handler;   //�����ʱ���¼�������
} app_screen_page_t, *app_screen_page_pt;

app_screen_page_pt current_page;

#define FORWARD_PAGE 1
#define BACKWARD_PAGE 2

#define VISIBILITY_VISIBLE 1
#define VISIBILITY_HIDDEN 0

#define APP_SCREEN_HISTORY_MAX_SIZE 30
static app_screen_page_pt app_screen_histories[APP_SCREEN_HISTORY_MAX_SIZE];
volatile uint16_t app_screen_history_idx = 0;
static int app_screen_history_push(app_screen_page_pt page)
{
    if (app_screen_history_idx >= APP_SCREEN_HISTORY_MAX_SIZE)
        return APP_ERROR;
    app_screen_histories[app_screen_history_idx] = page;
    app_screen_history_idx++;
    return APP_OK;
}
static int app_screen_history_pop(app_screen_page_pt *page)
{
    if (app_screen_history_idx == 0)
        return APP_ERROR;
    *page = app_screen_histories[--app_screen_history_idx];
    return APP_OK;
}
static int app_screen_history_reset()
{
    app_screen_history_idx = 0;
    return APP_OK;
}

static void go_to_main_page(void);
static int check_page_valid(app_screen_page_pt page)
{
    if (page->check != NULL && (!page->check(page)))
    {
        go_to_main_page();
        return 0;
    }
    return 1;
}

void redirct_page(app_screen_page_pt to_page, app_screen_page_pt from_page, uint8_t direct)
{
    if (to_page == NULL)
    {
        return;
    }
    if (to_page == current_page)
    {
        return;
    }
    if (from_page != NULL)
    {
        if (direct == FORWARD_PAGE)
        {
            if (from_page->save_history != 0)
            {
                app_screen_history_push(from_page);
            }
        }
        if (from_page->after != NULL)
        {
            from_page->after(from_page);
        }
    }
    if (to_page->init != NULL && to_page->initialized == 0)
    {
        to_page->init(to_page);
        to_page->initialized = 1;
    }
    if (to_page->user_init != NULL && to_page->user_initialized == 0)
    {
        to_page->user_init(to_page);
        to_page->user_initialized = 1;
    }
    if (!check_page_valid(to_page))
    {
        return;
    }
    if (to_page->before != NULL)
    {
        to_page->before(to_page, from_page);
    }
    if (to_page->before_render != NULL && direct == FORWARD_PAGE)
    {
        to_page->before_render(to_page);
    }
    current_page = NULL;
    BSP_LCD12864_clear_txt();
    to_page->render(to_page);

    current_page = to_page;
}

void backward_page(app_screen_page_pt page)
{
    app_screen_page_pt back;
    int rc = app_screen_history_pop(&back);
    if (rc != APP_OK || back == NULL || page == NULL)
    {
        return;
    }
    redirct_page(back, page, BACKWARD_PAGE);
}

#define forward_page(to_page, from_page) redirct_page(to_page, from_page, FORWARD_PAGE);

#endif
