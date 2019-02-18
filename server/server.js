const app = require('./config/express')();
const config = require('./config/config')();
const logger = require('./app/utils/logger');

// Initialize the app
app.listen(config.PORT_HTTPS, () => {
  logger.success('C- Compiler Server Running on Port ' + config.PORT_HTTPS);
});
