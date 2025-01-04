FROM openjdk:17-jdk-slim

WORKDIR /app

COPY target/mod_web-client-0.0.1-SNAPSHOT.jar app.jar

EXPOSE 10800

ENTRYPOINT ["java", "-jar", "app.jar"]
