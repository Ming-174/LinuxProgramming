#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
typedef struct task_t {
	void(*func)(void*);
	void* arg;
	struct task_t* next;
}task_t;

typedef struct pool_t {
	task_t* head;
	task_t* tail;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}pool_t;

void* work(void* arg) {
	//不用把pool解析放进循环，线程间数据共享任务队列
	pool_t* pool = (pool_t*)arg;
	while (1) {
		pthread_mutex_lock(&pool->mutex);
		//队列为空就睡觉
		while (pool->head == NULL) {
			pthread_cond_wait(&pool->cond, &pool->mutex);
		}
		task_t* t = pool->head;

		pool->head = t->next;
		//如果这是最后一个任务，头尾指针就都同时指向他，而这时候我们还需要手动重置尾指针
		if (pool->head == NULL)pool->tail = NULL;
		//任务队列操作完成，归还锁，线程本身也进入工作但不占用锁，让其他线程获取任务工作
		pthread_mutex_unlock(&pool->mutex);
		if (t->func == NULL && t->arg == NULL) {
			free(t);
			break;
		}

		t->func(t->arg);
		free(t);
	}
	pthread_exit("Pthread Exit");
}

void pt(void* arg) {
	char* buf = (char*)arg;
	write(STDOUT_FILENO, buf, strlen(buf) );
}
//为了可持续发展，submit不能只绑定一个函数，而是提供一个共用的接口，参数就需要有任务函数，任务参数
void submit(void(*func)(void*), void* arg, pool_t* pool) {
	task_t* t = (task_t*)malloc(sizeof(task_t));
	t->func = func;
	t->arg = arg;
	t->next = NULL;
	//提交任务，任务队列只能被一个线程操作，防止数据竞争
	pthread_mutex_lock(&pool->mutex);
	if (pool->head == NULL) {
		pool->head = t;
		pool->tail = t;
	}
	else {
		//这是先让t接入任务队列
		pool->tail->next = t;
		//再让尾部指针指向最后一个任务，便于继续接任务
		pool->tail = t;
	}
	//接完任务赶紧还锁，让线程运行，避免浪费CPU
	pthread_cond_signal(&pool->cond);
	pthread_mutex_unlock(&pool->mutex);
}

void posion(pool_t* pool) {
	//NULL,NULL便是毒杀信号
	submit(NULL,NULL,pool);
}

int main() {
	pool_t pool;
	pool.head = NULL;
	pool.tail = NULL;
	pthread_mutex_init(&pool.mutex, NULL);
	pthread_cond_init(&pool.cond, NULL);
	pthread_t tid[3];
	for (int i = 0; i < 3; i++) {
		pthread_create(&tid[i], NULL, work, &pool);
	}

	char* buf[] = {
		"Beth ",
		"Harmon ",
		"Always ",
		"Love ",
		"You!"
	};
	for (int i = 0; i < 5; i++) {
		submit(pt, buf[i], &pool);
	}
	//任务全部提交，开始投毒
	for (int i = 0; i < 3; i++) {
		posion(&pool);
	}
	for (int i = 0; i < 3; i++){
		pthread_join(tid[i], NULL);
	}
	pthread_cond_destroy(&pool.cond);
	pthread_mutex_destroy(&pool.mutex);
}
