module.exports = (app) => {
  const { compiler } = app.controllers;
  app.post('/compile', compiler.compile);
};
