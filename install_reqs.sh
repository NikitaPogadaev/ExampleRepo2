#!/bin/bash
# sudo apt-get install libboost-all-dev
#



check_package() {
    dpkg -s "$1" >/dev/null 2>&1
}


if ! check_package "libboost-all-dev" ; then
    echo "Библиотека Boost не установлена, начинаем установку..."

    sudo apt-get update
    sudo apt-get install libboost-all-dev

    if check_package "libboost-all-dev" ; then
        echo "Библиотека Boost успешно установлена."
    else
        echo "Ошибка при установке библиотеки Boost."
        exit 1
    fi
else
    echo "Библиотека Boost уже установлена."
fi


# Проверяем наличие Google Test
if ! check_package "libgtest-dev"; then
    echo "Google Test не установлен, начинаем установку..."
    sudo apt-get update
    sudo apt-get install -y libgtest-dev
else
    echo "Google Test уже установлен."
fi

# Проверяем наличие Google Mock
if ! check_package "libgmock-dev"; then
    echo "Google Mock не установлен, начинаем установку..."
    sudo apt-get update
    sudo apt-get install -y libgmock-dev
else
    echo "Google Mock уже установлен."
fi
