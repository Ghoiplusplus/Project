# Используем образ с Maven и JDK для сборки
FROM maven:3.9.9-eclipse-temurin-17 as build

# Устанавливаем рабочую директорию
WORKDIR /app

# Копируем файлы проекта
COPY pom.xml .
COPY src ./src

# Собираем проект
RUN mvn clean package -DskipTests

# Используем минимальный образ с OpenJDK для запуска
FROM openjdk:17-jdk-slim

# Устанавливаем рабочую директорию
WORKDIR /app

# Копируем собранный JAR из предыдущего этапа
COPY --from=build /app/target/mod_web-client-0.0.1-SNAPSHOT.jar app.jar

# Открываем порт
EXPOSE 10800

# Запускаем приложение
ENTRYPOINT ["java", "-jar", "app.jar"]
