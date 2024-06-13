function createDatatype(datatypeName, variantName) {
  return (...values) => [null, datatypeName, variantName, ...values];
}

function callLambda(lambda) {
  const env = lambda["0"];
  const handler = lambda["1"];

  return (...args) => handler(env, ...args);
}

const None = createDatatype('Option', 'None');
const Some = createDatatype('Option', 'Some');

const tuple = createDatatype('tuple', 'tuple');

const child_process = require('child_process');
const fs = require('fs');

module.exports = {
  ppm_writefile: (path, content) => {
    fs.writeFileSync(path, content);
  },

  ppm_mkdir: (path) => {
    fs.mkdirSync(path);
  },
  get_cwd: () => Some(process.cwd()),
  get_env(key) {
    const res = process.env[key];
    return res ? Some(res) : None();
  },
  ppm_chmod: (path, mode) => {
    fs.chmodSync(path, mode);
  },
  does_directory_exist(path) {
    return fs.existsSync(path) && fs.lstatSync(path).isDirectory();
  },

  does_file_exist_sync(path) {
    return fs.existsSync(path) && fs.lstatSync(path).isFile();
  },

  async spawn_process(cmd, args) {
    return new Promise((resolve) => {
      const child = child_process.spawn(cmd, args);
      let stdout = '';
      let stderr = '';

      child.stdout.on('data', (data) => {
        stdout += data;
      });

      child.stderr.on('data', (data) => {
        stderr += data;
      });

      child.on('close', (code) => 
        resolve(tuple(code, stdout.trim(), stderr.trim()))
      );
    });
  },

  async spawn_process_callback(cmd, args, callback) {
    return new Promise((resolve) => {
      const child = child_process.spawn(cmd, args);
      let stdout = '';
      let stderr = '';

      child.stdout.on('data', (data) => {
        callLambda(callback)(data);
        stdout += data;
      });

      child.stderr.on('data', (data) => {
        stderr += data;
      });

      child.on('close', (code) => 
        resolve(tuple(code, stdout.trim(), stderr.trim()))
      );
    });
  },

  async spawn_process_to_terminal(cmd, args) {
    return new Promise((resolve) => {
      const child = child_process.spawn(cmd, args);
      let stdout = '';
      let stderr = '';

      child.stdout.pipe(process.stdout);
      child.stderr.pipe(process.stderr);

      child.stdout.on('data', (data) => {
        stdout += data;
      });

      child.stderr.on('data', (data) => {
        stderr += data;
      });

      child.on('close', (code) => 
        resolve(tuple(code, stdout.trim(), stderr.trim()))
      );
    });
  },
}