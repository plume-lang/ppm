function None() {
  return [null, 'Option', 'None'];
}

const child_process = require('child_process');

function Some(x) {
  return [null, 'Option', 'Some', x];
}

const fs = require('fs');

module.exports = {
  is_alphabetic: (c) => 
    /[a-zA-Z]/.test(c),
  is_digit: (c) =>
    /[0-9]/.test(c),
  is_alphanumeric: (c) =>
    /[a-zA-Z]|[0-9]/.test(c),
  is_whitespace: (c) =>
    /\s/.test(c),
  ffi_to_int: (s) => parseInt(s),
  str_split: (s, sep) => s.split(sep),
  which: (s) => {
    const fs = require('fs');
    return fs.existsSync(s) ? s : '';
  },
  str_index(s, i) {
    if (i < 0 || i >= s.length) {
      return None();
    }

    return Some(s[i]);
  },

  explode(s) {
    return s.split('');
  },

  debug: (s) => console.log(s),

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

  async spawn_process(cmd, args) {
    return new Promise((resolve, reject) => {
      const child = child_process.spawn(cmd, args);
      let stdout = '';
      let stderr = '';

      child.stdout.on('data', (data) => {
        stdout += data;
      });

      child.stderr.on('data', (data) => {
        stderr += data;
      });

      child.on('close', (code) => {
        if (code === 0) {
          resolve([null, 'tuple', 'tuple', code, stdout.trim()]);
        } else {
          reject([null, 'tuple', 'tuple', code, stderr]);
        }
      });
    });
  }
}