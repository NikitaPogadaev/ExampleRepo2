#!/usr/bin/python3

import unittest

# Функция для тестирования (сложение двух чисел)
def add(a, b):
    return a + b

# Класс для тестирования функции add
class TestAddFunction(unittest.TestCase):

    # Метод для тестирования ситуации, когда оба аргумента положительные
    def test_add_positive_numbers(self):
        result = add(3, 5)
        self.assertEqual(result, 8)  # Проверяем, что результат равен 8

    # Метод для тестирования ситуации, когда один из аргументов отрицательный
    def test_add_negative_number(self):
        result = add(-3, 7)
        self.assertEqual(result, 4)  # Проверяем, что результат равен 4

    # Метод для тестирования ситуации, когда оба аргумента отрицательные
    def test_add_both_negative_numbers(self):
        result = add(-3, -5)
        self.assertEqual(result, -8)  # Проверяем, что результат равен -8

    # Метод для тестирования ситуации, когда оба аргумента равны 0
    def test_add_both_zero(self):
        result = add(0, 0)
        self.assertEqual(result, 0)  # Проверяем, что результат равен 0

# Запуск тестов
if __name__ == '__main__':
    unittest.main()
