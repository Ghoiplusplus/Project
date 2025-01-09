// init.js
db = db.getSiblingDB('userdb'); // Создаём или переключаемся на базу userdb
db.createCollection('users');  // Создаём коллекцию users
print('Инициализация MongoDB завершена');