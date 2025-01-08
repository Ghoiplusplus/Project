# Шаг 1: Сборка Go-приложения
FROM golang:1.23 AS builder

# Устанавливаем рабочую директорию
WORKDIR /app

# Копируем файлы зависимостей
COPY go.mod go.sum ./
RUN go mod download

# Копируем все исходные файлы проекта
COPY . .

# Компилируем приложение
RUN go build -o main .

# Шаг 2: Используем образ ubuntu - alpine образ не совместим с этим бинарным файлом
FROM ubuntu:22.04

# Устанавливаем необходимые зависимости
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Копируем исполняемый файл и файл окружения
COPY --from=builder /app/main /main
COPY secret.env /secret.env

# Делаем файл исполняемым
RUN chmod +x /main

# Открываем порт приложения
EXPOSE 2228

# Команда запуска
CMD ["/main"]
