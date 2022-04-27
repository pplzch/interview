#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <set>

std::mutex mutex;
std::condition_variable cv;

typedef struct Row
{
	int a;
	int b;
} Row;

/*!
* Find out all the rows that sastify below conditions:
*
* ((b >= 10 && b < 50) &&
* (a == 1000 || a == 2000 || a == 3000))
*
* @param nrows The total number of rows.
* @param rows The rows, for example rows[0] is the first row.
*/
void
conditional_print_task1(const Row *rows, int nrows)
{
	/* Save the pointer which will be printed, when we failed to acquire the lock. */
	std::vector<Row *> tmp_vec;

	for (int i = 0; i < nrows; ++i)
	{
		if ((rows[i].a == 1000 || rows[i].a == 2000 || rows[i].a == 3000) &&
				(rows[i].b >= 10 && rows[i].b < 50))
		{
					if (mutex.try_lock())
					{
						if (tmp_vec.size() == 0)
							std::cout << rows[i].a << " " << rows[i].b << std::endl;
						else
						{
							for (auto ptr : tmp_vec)
							{
								std::cout << ptr->a << " " << ptr->b << std::endl;
							}
							tmp_vec.clear();
						}

						mutex.unlock();
					}
					else
						tmp_vec.push_back(&rows[i]);
		}
	}

	if (tmp_vec.size() != 0)
	{
		mutex.lock();
		for (auto ptr : tmp_vec)
		{
			std::cout << ptr->a << " " << ptr->b << std::endl;
		}
		mutex.unlock();
	}
}

/*!
* Task 1.
*
* Find out all the rows that sastify below conditions:
*
* ((b >= 10 && b < 50) &&
* (a == 1000 || a == 2000 || a == 3000))
*
* Print them to the terminal, one row per line, for example:
*
* 1000,20
* 1000,23
* 2000,16
*
* @param nrows The total number of rows.
* @param rows The rows, for example rows[0] is the first row.
*/
void
task1(const Row *rows, int nrows)
{
	std::vector<std::thread> threads;
	/* Get the number of the cores. */
	long int cores = sysconf(_SC_NPROCESSORS_ONLN);
	const Row *arg_rows =	rows;
	for (int i = 0; i < cores; ++i)
	{
		const Row *arg_rows =	arg_rows + i * nrows / cores;
		int arg_nrows =	nrows / cores + (i == cores - 1 ? nrows % cores : 0);
	
		threads.push_back(conditional_print_task1, arg_rows, arg_nrows);
	}

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

/*!
* Find out all the rows that sastify below conditions:
*
* (a == 1000 || a == 2000 || a == 3000))
*
* @param nrows The total number of rows.
* @param rows The rows, for example rows[0] is the first row.
*/
void
conditional_print_task2(const Row *rows, int nrows)
{
	/* Save the pointer which will be printed, when we failed to acquire the lock. */
	std::vector<Row *> tmp_vec;

	for (int i = 0; i < nrows; ++i)
	{
		if (rows[i].a == 1000 || rows[i].a == 2000 || rows[i].a == 3000)
		{
					if (mutex.try_lock())
					{
						if (tmp_vec.size() == 0)
							std::cout << rows[i].a << " " << rows[i].b << std::endl;
						else
						{
							for (auto ptr : tmp_vec)
							{
								std::cout << ptr->a << " " << ptr->b << std::endl;
							}
							tmp_vec.clear();
						}

						mutex.unlock();
					}
					else
						tmp_vec.push_back(&rows[i]);
		}
	}

	if (tmp_vec.size() != 0)
	{
		mutex.lock();
		for (auto ptr : tmp_vec)
		{
			std::cout << ptr->a << " " << ptr->b << std::endl;
		}
		mutex.unlock();
	}
}

/*!
* Find out the smallest index of target in rows.b.
*
*
* @param nrows The total number of rows.
* @param rows The rows, for example rows[0] is the first row.
* @param target The target value.
*
* @return index if found, else -1.
*/
int
binary_search_smallest_index(const Row *rows, int nrows, int target)
{
	int l = 0;
	int r = nrows - 1;
	while (l <= r) {
		int mid = (l + r) / 2;
		if (rows[mid].b == target && rows[mid - 1].b < rows[mid].b) return mid;
		if (target < rows[mid].b || (t == rows[mid].b && target == rows[mid - 1].b))
			r = mid - 1;
		else
			l = mid + 1;
	}

	return -1;
}

/*!
* Task 2.
*
* Find out all the rows that sastify below conditions, if the rows is ordered:
*
* ((b >= 10 && b < 50) &&
* (a == 1000 || a == 2000 || a == 3000))
*
* Print them to the terminal, one row per line, for example:
*
* 1000,20
* 1000,23
* 2000,16
*
* @param nrows The total number of rows.
* @param rows The rows, for example rows[0] is the first row.
*/
void
task2(const Row *rows, int nrows)
{
	/* Get the interval between b >= 10 && b < 50 */
	int beg = binary_search_smallest_index(rows, nrows, 10);
	int end = binary_search_smallest_index(rows, nrows, 50);

	if (end == -1)
		end = nrows - 1;
	else
		end = end - 1;

	rows = rows + beg;
	nrows = end - beg + 1;

	std::vector<std::thread> threads;
	/* Get the number of the cores. */
	long int cores = sysconf(_SC_NPROCESSORS_ONLN);
	const Row *arg_rows =	rows;
	for (int i = 0; i < cores; ++i)
	{
		const Row *arg_rows =	arg_rows + i * nrows / cores;
		int arg_nrows =	nrows / cores + (i == cores - 1 ? nrows % cores : 0);
	
		threads.push_back(conditional_print_task2, arg_rows, arg_nrows);
	}

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int indicator_task_3 = 0;
/*!
* Find out all the rows in order that sastify below conditions:
*
* (a == 1000 || a == 2000 || a == 3000))
*
* @param nrows The total number of rows.
* @param rows The rows, for example rows[0] is the first row.
* @param order Print order indicator
*/
void
conditional_print_task3(const Row *rows, int nrows, int order)
{
	std::unique_lock<std::mutex> lock(mutex);
	while (indicator_task_3 != order)
		cv.wait(lock);

	for (int i = 0; i < nrows; ++i)
	{
		if (rows[i].a == 1000 || rows[i].a == 2000 || rows[i].a == 3000)
			std::cout << rows[i].a << " " << rows[i].b << std::endl;
	}

	++indicator_task_3;
	cv.notify_all();
}

/*!
* Task 3.
*
* Find out all the rows that sastify below conditions, if the rows is ordered:
*
* ((b >= 10 && b < 50) &&
* (a == 1000 || a == 2000 || a == 3000))
*
* Print them to the terminal, one row per line in order, for example:
*
* 1000,20
* 1000,23
* 2000,16
*
* @param nrows The total number of rows.
* @param rows The rows, for example rows[0] is the first row.
*/
void
task3(const Row *rows, int nrows)
{
	/* Get the interval between b >= 10 && b < 50 */
	int beg = binary_search_smallest_index(rows, nrows, 10);
	int end = binary_search_smallest_index(rows, nrows, 50);

	if (end == -1)
		end = nrows - 1;
	else
		end = end - 1;

	rows = rows + beg;
	nrows = end - beg + 1;

	std::vector<std::thread> threads;
	/* Get the number of the cores. */
	long int cores = sysconf(_SC_NPROCESSORS_ONLN);
	const Row *arg_rows =	rows;
	for (int i = 0; i < cores; ++i)
	{
		const Row *arg_rows =	arg_rows + i * nrows / cores;
		int arg_nrows =	nrows / cores + (i == cores - 1 ? nrows % cores : 0);
	
		threads.push_back(conditional_print_task3, arg_rows, arg_nrows, i);
	}

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int indicator_task_4 = 0;
/*!
* Find out all the rows in order that sastify below conditions:
*
* (a == 1000 || a == 2000 || a == 3000))
*
* @param nrows The total number of rows.
* @param rows The rows, for example rows[0] is the first row.
* @param order Print order indicator
* @param s     set for the purpose value.
*/
void
conditional_print_task4(const Row *rows, int nrows, int order, std::set<int> &s)
{
	std::unique_lock<std::mutex> lock(mutex);
	while (indicator_task_4 != order)
		cv.wait(lock);

	for (int i = 0; i < nrows; ++i)
	{
		if (s.find(rows[i].a) != s.end())
			std::cout << rows[i].a << " " << rows[i].b << std::endl;
	}

	++indicator_task_4;
	cv.notify_all();
}

/*!
* Task 4.
*
* Find out all the rows that sastify below conditions, if the rows is ordered:
*
* ((b >= 10 && b < 50) &&
* (a == 1000 || a == 2000 || a == 3000 || ... || a == 98000 || a == 99000))
*
* Print them to the terminal, one row per line in order, for example:
*
* 1000,20
* 1000,23
* 2000,16
*
* @param nrows The total number of rows.
* @param rows The rows, for example rows[0] is the first row.
*/
void
task4(const Row *rows, int nrows)
{
	/* Get the interval between b >= 10 && b < 50 */
	int beg = binary_search_smallest_index(rows, nrows, 10);
	int end = binary_search_smallest_index(rows, nrows, 50);

	if (end == -1)
		end = nrows - 1;
	else
		end = end - 1;

	rows = rows + beg;
	nrows = end - beg + 1;

	std::set<int> s;
	int val = 1000;
	while (val <= 98000)
	{
		s.insert(val);
		val += 1000;
	}

	std::vector<std::thread> threads;
	/* Get the number of the cores. */
	long int cores = sysconf(_SC_NPROCESSORS_ONLN);
	const Row *arg_rows =	rows;
	for (int i = 0; i < cores; ++i)
	{
		const Row *arg_rows =	arg_rows + i * nrows / cores;
		int arg_nrows =	nrows / cores + (i == cores - 1 ? nrows % cores : 0);
	
		threads.push_back(conditional_print_task4, arg_rows, arg_nrows, i, s);
	}

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}
