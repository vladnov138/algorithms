#pragma once

#include <iostream>
#include <type_traits>
#include <time.h>

class TypeException {};
class SizeException {};
class DimensionException {};

template <class T>
class NDArray
{
private:
	T* arr;
	int* arr_size;
	int size;
	int dimension;
public:
	NDArray(const NDArray& arr_a);
	NDArray(int* arr_size, int arr_size_length);
	~NDArray();
	void fill_zero();
	void fill_identity();
	void fill_random();
	T get_item(const int* index, int index_size);
	void set_item(const int* index, int index_size, const T& value);
	NDArray<T> operator + (const NDArray& arr_a);
	NDArray<T>& operator += (const NDArray<T> arr_a);
	NDArray<T> operator - (const NDArray<T> arr_a);
	NDArray<T>& operator -= (const NDArray<T> arr_a);
	NDArray<T> operator / (const NDArray<T> arr_a);
	NDArray<T>& operator /= (const NDArray<T> arr_a);
	NDArray<T> operator * (const NDArray<T> arr_a);
	NDArray<T>& operator *= (const NDArray<T> arr_a);
	NDArray<T> matmul(NDArray<T> arr_a);
	NDArray transpose();
	T* min(int axis);
	T min();
	T* max(int axis);
	T max();
	double* mean(int axis);
	double mean();
};

template<class T>
NDArray<T>::NDArray(int* arr_size, int arr_size_length) {
	if (!std::is_same_v<T, float> && !std::is_same_v<T, int>)
		throw TypeException();
	int length = arr_size_length;
	dimension = length;
	this->arr_size = new int[length];
	size = 1;
	for (int i = 0; i < length; i++) {
		size *= arr_size[i];
		this->arr_size[i] = arr_size[i];
	}
	arr = new T[size];
}

template<class T>
NDArray<T>::NDArray(const NDArray& arr_a) {
	size = arr_a.size;
	dimension = arr_a.dimension;
	arr_size = new int[dimension];
	arr = new int[size];
	for (int i = 0; i < dimension; i++)
		arr_size[i] = arr_a.arr_size[i];
	for (int i = 0; i < size; i++)
		arr[i] = arr_a.arr[i];
}

template <class T>
NDArray<T>::~NDArray() {
	delete[] arr;
	delete[] arr_size;
}

template<class T>
void NDArray<T>::fill_zero() {
	for (int i = 0; i < size; i++)
		arr[i] = 0;
}

template<class T>
void NDArray<T>::fill_identity() {
	for (int i = 0; i < size; i++)
		arr[i] = 1;
}

template<class T>
void NDArray<T>::fill_random() {
	srand((unsigned)time(0));
	for (int i = 0; i < size; i++)
		arr[i] = rand();
}

template<class T>
NDArray<T> NDArray<T>::operator+(const NDArray& arr_a) {
	if (size != arr_a.size)
		throw SizeException();
	return NDArray<T>(*this) += arr_a;
}

template<class T>
NDArray<T>& NDArray<T>::operator+=(const NDArray<T> arr_a) {
	if (arr_size[0] != arr_a.arr_size[0] || arr_size[1] != arr_a.arr_size[1])
		throw SizeException();
	for (int i = 0; i < size; i++)
		arr[i] += arr_a.arr[i];
	return *this;
}

template<class T>
NDArray<T> NDArray<T>::operator-(const NDArray<T> arr_a) {
	if (size != arr_a.size)
		throw SizeException();
	return NDArray<T>(*this) -= arr_a;
}

template<class T>
NDArray<T>& NDArray<T>::operator-=(const NDArray<T> arr_a) {
	if (size != arr_a.size)
		throw SizeException();
	for (int i = 0; i < size; i++)
		arr[i] -= arr_a.arr[i];
	return *this;
}

template<class T>
NDArray<T> NDArray<T>::operator/(const NDArray<T> arr_a) {
	if (size != arr_a.size)
		throw SizeException();
	return NDArray<T>(*this) /= arr_a;
}

template<class T>
NDArray<T>& NDArray<T>::operator/=(const NDArray<T> arr_a) {
	if (size != arr_a.size)
		throw SizeException();
	for (int i = 0; i < size; i++)
		arr[i] /= arr_a.arr[i];
	return *this;
}

template<class T>
NDArray<T> NDArray<T>::operator*(const NDArray<T> arr_a) {
	if (size != arr_a.size)
		throw SizeException();
	return NDArray<T>(*this) *= arr_a;
}

template<class T>
NDArray<T>& NDArray<T>::operator*=(const NDArray<T> arr_a) {
	if (size != arr_a.size)
		throw SizeException();
	for (int i = 0; i < size; i++)
		arr[i] *= arr_a.arr[i];
	return *this;
}

template<class T>
T NDArray<T>::get_item(const int* index, int index_length) {
	if (dimension != index_length)
		throw DimensionException();
	int arr_idx = 0;
	for (int i = 0; i + 1 < index_length; i++) {
		if (index[i] >= arr_size[i])
			throw SizeException();
		arr_idx += index[i] * arr_size[i + 1];

	}
	if (index[index_length - 1] >= arr_size[index_length - 1])
		throw SizeException();
	arr_idx += index[index_length - 1];
	return arr[arr_idx];
}

template<class T>
void NDArray<T>::set_item(const int* index, int index_length, const T& value) {
	if (dimension != index_length)
		throw DimensionException();
	int arr_idx = 0;
	for (int i = 0; i + 1 < index_length; i++) { 
		if (index[i] >= arr_size[i])
			throw SizeException();
		arr_idx += index[i] * arr_size[i + 1];
	}
	if (index[index_length - 1] >= arr_size[index_length - 1])
		throw SizeException();
	arr_idx += index[index_length - 1];
	arr[arr_idx] = value;
}

template<class T>
NDArray<T> NDArray<T>::matmul(NDArray<T> arr_a) {
	if (dimension != 2 || arr_size[0] != arr_a.arr_size[1] || arr_size[1] != arr_a.arr_size[0])
		throw DimensionException();
	int size[] = { arr_size[0], arr_a.arr_size[1] };
	NDArray<T> result = NDArray<T>(size, sizeof(size) / sizeof(size[0]));
	for (int i = 0; i < size[0]; i++) {
		for (int j = 0; j < size[1]; j++) {
			int idx[] = { i, j };
			int mul_res = 0;
			for (int k = 0; k < arr_size[1]; k++) {
				int idx1[] = { i, k };
				int idx2[] = { k, i };
				mul_res += get_item(idx1, dimension) * arr_a.get_item(idx2, dimension);
			}
			result.set_item(idx, dimension, mul_res);
		}
	}
	return result;
}

template<class T>
NDArray<T> NDArray<T>::transpose() {
	if (dimension != 2)
		throw DimensionException();
	int* new_size = new int[2];
	new_size[1] = arr_size[0];
	new_size[0] = arr_size[1];
	NDArray<T> result(new_size, dimension);
	result.fill_identity();
	for (int i = 0; i < new_size[0]; i++)
		for (int j = 0; j < new_size[1]; j++) {
			int idx[] = { i, j };
			int inverse_idx[] = { j, i };
			result.set_item(idx, dimension, get_item(inverse_idx, dimension));
		}
	return result;
}

template<class T>
T* NDArray<T>::min(int axis) {
	if (dimension != 2)
		throw DimensionException();
	T* min_result;
	switch (axis) {
	case 0:	
		min_result = new T[arr_size[0]];
		for (int i = 0; i < arr_size[1]; i++) {
			int idx[] = { i, 0 };
			min_result[i] = get_item(idx);
			for (int j = 0; j < arr_size[0]; j++) {
				idx[0] = j; idx[1] = i;
				if (get_item(idx) < min_result[i])
					min_result[i] = get_item(idx);
			}
		}
		break;
	case 1:
		min_result = new T[arr_size[1]];
		for (int i = 0; i < arr_size[0]; i++) {
			int idx[] = { i, 0 };
			min_result[i] = get_item(idx);
			for (int j = 0; j < arr_size[1]; j++) {
				idx[0] = i; idx[1] = j;
				if (get_item(idx) < min_result[i])
					min_result[i] = get_item(idx);
			}
		}
		break;
	default:
		throw DimensionException();
	}
	return min_result;
}

template<class T>
T NDArray<T>::min() {
	T min_value = arr[0];
	for (int i = 0; i < size; i++) {
			if (arr[i] < min_value)
				min_value = arr[i];
		}
	return min_value;
}

template<class T>
T* NDArray<T>::max(int axis) {
	if (dimension != 2)
		throw DimensionException();
	T* max_result;
	switch (axis) {
	case 0:
		max_result = new T[arr_size[0]];
		for (int i = 0; i < arr_size[1]; i++) {
			int idx[] = { i, 0 };
			max_result[i] = get_item(idx);
			for (int j = 0; j < arr_size[0]; j++) {
				idx[0] = j; idx[1] = i;
				if (get_item(idx) > max_result[i])
					max_result[i] = get_item(idx);
			}
		}
		break;
	case 1:
		max_result = new T[arr_size[1]];
		for (int i = 0; i < arr_size[0]; i++) {
			int idx[] = { i, 0 };
			max_result[i] = get_item(idx);
			for (int j = 0; j < arr_size[1]; j++) {
				idx[0] = i; idx[1] = j;
				if (get_item(idx) > max_result[i])
					max_result[i] = get_item(idx);
			}
		}
		break;
	default:
		throw DimensionException();
	}
	return max_result;
}

template<class T>
T NDArray<T>::max() {
	T max_value = arr[0];
	for (int i = 0; i < size; i++) {
			if (arr[i] > max_value)
				max_value = arr[i];
		}
	return max_value;
}

template<class T>
double* NDArray<T>::mean(int axis) {
	if (dimension != 2)
		throw DimensionException();
	double* mean_result;
	switch (axis) {
	case 0:
		mean_result = new double[arr_size[1]];
		for (int i = 0; i < arr_size[1]; i++) {
			int s = 0;
			for (int j = 0; j < arr_size[0]; j++) {
				int idx[] = { j, i };
				s += get_item(idx, dimension);
			}
			mean_result[i] = (double) s / arr_size[0];
		}
		break;
	case 1:
		mean_result = new double[arr_size[1]];
		for (int i = 0; i < arr_size[0]; i++) {
			int s = 0;
			for (int j = 0; j < arr_size[1]; j++) {
				int idx[] = { i, j };
				s += get_item(idx, dimension);
			}
			mean_result[i] = (double) s / arr_size[1];
		}
		break;
	default:
		throw DimensionException();
	}
	return mean_result;
}

template<class T>
double NDArray<T>::mean() {
	double mean_value = 0;
	for (int i = 0; i < size; i++)
		mean_value += arr[i];
	return mean_value / size;
}