const sqlite3 = require('sqlite3').verbose();
const path = require('path');

/** 
* 打开、关闭数据库连接
* DDL：创建表格，修改，删除
* DML:创建记录，修改，删除
* DQL:从一个或者多个表中查询数据
*/

function connectDatabase() {

    // 连接数据库，并创建数据库文件mydatebase.db
    // 打包为exe时目录结构有变化，unable to open database file
    var dir = path.resolve(__dirname, './config/mydatebase.db');
    console.log("数据库文件路径：", dir);
    var db = new sqlite3.Database(dir, (err) => {
        if (err) {
            throw err;
        }
        console.log('sqlite3已经连接成功')
        db.serialize(() => {
            //创建一个叫做 "user" 的新表。这个表包含四列：id, name, email以及phone。
            //id列是整数类型，并且是主键，具有自动递增的属性。名字、电子邮件和电话都是文本类型。
            db.run('create table if not exists users (id INTEGER PRIMARY KEY AUTOINCREMENT, name text, email text);', (err) => {
                if(err) console.error(err.message);
            });

            // 插入数据
            let stmt = db.prepare('INSERT INTO users (name, email) VALUES (?, ?)');
            for (let i = 0; i < 10; i++) {
                stmt.run(`user${i}`, `user${i}@example.com`);
            }
            stmt.finalize();
            
            db.run(
                `INSERT INTO users(name, email) VALUES('张三', 'san.zhang@163.com')`,
                function (err) {
                    if (err) throw err;
                    else {
                        console.log('写入数据ID', this.lastID);
                    }
                }
            );

            // 改
            db.run('UPDATE users SET name = ? WHERE id = ?', ['newName', 1], (err) => {
                if(err) console.error(err.message);
            });
            
            // 删
            db.run('DELETE FROM users WHERE id = ?', [1], (err) => {
                if(err) console.error(err.message);
            });
            
            // 修改表
            db.run('ALTER TABLE users ADD COLUMN age integer', (err) => {
                if(err) console.error(err.message);
            });
            
            //查询所有数据
            db.all('SELECT * FROM users', (err, rows) => {
                if (err) throw err
                else {
                    console.log('查询结果：', rows)
                }
            });

            // 逐条查询数据,每一段会单独打印
            // db.each("select * from users where name=?", 'miao', function(err, row) {
            //     if (err) throw err
            //     else {
            //         console.log('按条件查询结果是： ', row)
            //     }
            // })

            // 删除表
            db.run('DROP TABLE users', (err) => {
                if(err) console.error(err.message);
            });
            
            // 关闭数据库
            db.close((err) => {
                if (err) {
                    console.error(err.message);
                }
                console.log('Close the database connection.');
            });
        });        
    });
}

exports.db = connectDatabase;