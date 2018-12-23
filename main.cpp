// promiseFuture.cpp

#include <glib.h>
#include <gmodule.h>
#include <gio/gio.h>
#include <time.h>

#include <future>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>
#include <chrono>

std::string NameArray[10] = {
	"budz" ,
	"pain" ,
	"konan",
	"nagato",
	"itachi",
	"tobi",
	"madara",
	"naruto",
	"danzou",
	"kakashi"
};

typedef struct main_struct {
	GMainLoop *mainloop;
	std::vector<std::shared_future<int>> f;
} main_struct;

void product(std::promise<int>&& p, std::string name, int t)
{
	int a = rand() % 10 + 1;
	int b = rand() % 10 + 1;
	std::cout << name << " start, waiting " << t << std::endl;

	sleep(t);
	p.set_value(a*b);
	std::cout << name << " ready!!" << std::endl;
}

static gboolean sum_all(gpointer user_data)
{
	main_struct *m = (main_struct *)user_data;
	int sum = 0;

	// check if promise is finished
	for (auto f : m->f)
		if (f.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
			return TRUE;

	// get the result
	for (auto f : m->f)
		sum = sum + f.get();

	std::cout << "Sum all = " << sum << std::endl;
	g_main_loop_quit(m->mainloop);
	return FALSE;
}

int main()
{
	GSource *src = nullptr;

	srand(time(NULL));

	main_struct *m = g_new0(main_struct, 1);

	GMainContext *context = g_main_context_new();
	g_main_context_push_thread_default(context);

	m->mainloop = g_main_loop_new(context, TRUE);

	// get the futures
	for (int c = 0; c < 5; c++) {
		// define the promises
		std::promise<int> p;

		// get the futures
		std::shared_future<int> f = p.get_future();
		m->f.push_back(f);

		// calculate the result in a separat thread
		std::thread prodThread(product, std::move(p), NameArray[rand() % NameArray->size()], rand() % 10 + 1);
		prodThread.detach();
	}

	src = g_timeout_source_new(100);
	g_source_set_callback(src, (GSourceFunc)sum_all, m, nullptr);
	g_source_attach(src, context);
	g_source_unref(src);

	/* start the GLib event loop running */
	g_main_loop_run(m->mainloop);
	g_main_loop_unref(m->mainloop);
}