const express = require('express');
const mongoose = require('mongoose');
const multer = require('multer');
const jwt = require('jsonwebtoken');
const nodemailer = require('nodemailer');
const crypto = require('crypto');
const { GridFSBucket } = require('mongodb');

mongoose.connect('mongodb+srv://sanhector:salemmongo@hector.bjbn2.mongodb.net/?retryWrites=true&w=majority&appName=hector', { useNewUrlParser: true, useUnifiedTopology: true });
const conn = mongoose.connection;

const userSchema = new mongoose.Schema({
  email: { type: String, required: true, unique: true },
  otp: { type: String, default: null },
  otpExpiration: { type: Date, default: null },
  lastLoginTime: { type: Date, default: null }
});

const fileSchema = new mongoose.Schema({
  _id: mongoose.Schema.Types.ObjectId,
  name: { type: String, required: true },
  size: { type: Number, required: true },
  owner: { type: mongoose.Schema.Types.ObjectId, ref: 'User', required: true },
  type: { type: String, required: true, enum: ['TEXT', 'IMAGE', 'VIDEO', 'AUDIO', 'PDF', 'OTHER'] }
});

const invalidatedAccessTokenSchema = new mongoose.Schema({
  token: { type: String, required: true, unique: true },
  invalidatedAt: { type: Date, default: Date.now }
});

const User = mongoose.model('User', userSchema);
const File = mongoose.model('File', fileSchema);
const InvalidatedAccessToken = mongoose.model('InvalidatedAccessToken', invalidatedAccessTokenSchema);

let gfs;
conn.once('open', () => {
  gfs = new GridFSBucket(conn.db, { bucketName: 'files' });
});

const app = express();
app.use(express.json());
const storage = multer.memoryStorage();
const upload = multer({ storage });

const transporter = nodemailer.createTransport({
  service: 'gmail',
  auth: {
    user: 'planr.psg@gmail.com',
    pass: 'beohcufzvsfufxxw'
  }
});

const sendOtpEmail = (email, otp) => {
  const mailOptions = {
    from: 'Secure Vault',
    to: email,
    subject: 'OTP for Secure Vault',
    text: `Your OTP code is ${otp}. It is valid for 10 minutes.`
  };
  return transporter.sendMail(mailOptions);
};

const authenticate = async (req, res, next) => {
  const token = req.header('Authorization')?.replace('Bearer ', '');
  if (!token) return res.status(401).send('Access Denied');
  try {
    const invalidToken = await InvalidatedAccessToken.findOne({ token });
    if (invalidToken) return res.status(401).send('Invalid Token');
    const decoded = jwt.verify(token, 'securevaultkey');
    req.user = decoded;
    next();
  } catch (error) {
    res.status(400).send('Invalid Token');
  }
};

app.post('/sendotp', async (req, res) => {
  const { email } = req.body;
  const otp = Math.floor(100000 + Math.random() * 900000).toString();
  const expiration = Date.now() + 10 * 60 * 1000;
  let user = await User.findOne({ email });
  if (!user) {
    user = new User({ email, otp, otpExpiration: expiration });
    await user.save();
  } else {
    user.otp = otp;
    user.otpExpiration = expiration;
    await user.save();
  }
  try {
    await sendOtpEmail(email, otp);
    res.send('OTP sent');
  } catch (error) {
    res.status(500).send('Error sending OTP');
  }
});

app.post('/login', async (req, res) => {
  const { email, otp } = req.body;
  const user = await User.findOne({ email });
  if (!user) return res.status(400).send('Invalid credentials');
  if (user.otp !== otp || Date.now() > user.otpExpiration) return res.status(400).send('Invalid or expired OTP');
  user.otp = null;
  user.otpExpiration = null;
  user.lastLoginTime = new Date();
  await user.save();
  const token = jwt.sign({ id: user._id }, 'securevaultkey', { expiresIn: '1h' });
  res.json({ token });
});

app.post('/logout', authenticate, async (req, res) => {
  const token = req.header('Authorization')?.replace('Bearer ', '');
  await User.findByIdAndUpdate(req.user.id, { lastLoginTime: null });
  const invalidatedToken = new InvalidatedAccessToken({ token });
  await invalidatedToken.save();
  res.send('Logged out successfully');
});

const getFileType = (filename) => {
  const ext = filename.split('.').pop().toLowerCase();

  const textExtensions = [
    'txt', 'md', 'csv', 'json', 'xml', 'yaml', 'yml', 'ini', 'log', 'toml', 'cfg', 'conf',
    'html', 'htm', 'css', 'scss', 'sass', 'less',
    'js', 'mjs', 'cjs', 'ts', 'tsx', 'jsx', 
    'c', 'h', 'cpp', 'hpp', 'cc', 'cxx', 'ino',
    'java', 'kt', 'kts', 'scala', 'groovy', 'dart',
    'py', 'r', 'rb', 'pl', 'pm', 'php', 'phtml', 'sh', 'bash', 'zsh',
    'lua', 'swift', 'go', 'rs', 'jl', 'dart',
    'sql', 'psql', 'db', 'sqlite', 'ddl',
    'cs', 'fs', 'vb', 'vbs', 'aspx',
    'rkt', 'lisp', 'clj', 'cljs', 'scm', 'el', 'erl', 'ex', 'exs',
    'json5', 'properties', 'resx'
  ];
  
  const imageExtensions = ['jpg', 'jpeg', 'png', 'gif', 'bmp', 'webp', 'tiff', 'svg', 'ico', 'heic', 'avif'];
  const videoExtensions = ['mp4', 'avi', 'mkv', 'mov', 'wmv', 'flv', 'webm', 'm4v'];
  const audioExtensions = ['mp3', 'wav', 'aac', 'flac', 'ogg', 'm4a', 'opus', 'amr'];
  const documentExtensions = ['pdf', 'doc', 'docx', 'odt', 'rtf'];
  const archiveExtensions = ['zip', 'rar', '7z', 'tar', 'gz', 'bz2', 'xz'];

  if (textExtensions.includes(ext)) return 'TEXT';
  if (imageExtensions.includes(ext)) return 'IMAGE';
  if (videoExtensions.includes(ext)) return 'VIDEO';
  if (audioExtensions.includes(ext)) return 'AUDIO';
  if (documentExtensions.includes(ext)) return 'DOCUMENT';
  if (archiveExtensions.includes(ext)) return 'ARCHIVE';

  return 'OTHER';
};


app.post('/upload', authenticate, upload.single('file'), async (req, res) => {
  const fileId = new mongoose.Types.ObjectId();
  const uploadStream = gfs.openUploadStream(req.file.originalname, { id: fileId });
  uploadStream.end(req.file.buffer);
  uploadStream.on('finish', async () => {
    const file = new File({
      _id: fileId,
      name: req.file.originalname,
      size: req.file.size,
      owner: req.user.id,
      type: getFileType(req.file.originalname)
    });
    await file.save();
    res.send('File uploaded successfully');
  });
});

app.get('/download/:fileId', authenticate, async (req, res) => {
  try {
    const fileId = new mongoose.Types.ObjectId(req.params.fileId);
    const file = await File.findOne({ _id: fileId, owner: req.user.id });
    if (!file) return res.status(404).send('File not found');
    const downloadStream = gfs.openDownloadStream(fileId);
    downloadStream.on('error', () => res.status(500).send('Error downloading file'));
    res.setHeader('Content-Type', 'application/octet-stream');
    res.setHeader('Content-Disposition', `attachment; filename="${file.name}"`);
    downloadStream.pipe(res);
  } catch (err) {
    res.status(500).send('Internal Server Error');
  }
});

app.get('/list', authenticate, async (req, res) => {
  try {
    const filter = { owner: req.user.id };
    if (req.query.q) filter.name = { $regex: req.query.q, $options: 'i' };
    const files = await File.find(filter, 'name size _id type');
    res.json(files);
  } catch (error) {
    res.status(500).send('Internal Server Error');
  }
});

app.get('/info', authenticate, async (req, res) => {
  try {
    const fileId = new mongoose.Types.ObjectId(req.query.fileId);
    const file = await File.findOne({ _id: fileId, owner: req.user.id });
    if (!file) return res.status(404).send('File not found');
    res.json(file);
  } catch (error) {
    res.status(500).send('Internal Server Error');
  }
});

app.post('/rename', authenticate, async (req, res) => {
  const { fileId, newName } = req.body;
  const file = await File.findOne({ _id: fileId, owner: req.user.id });
  if (!file) return res.status(404).send('File not found');
  file.name = newName;
  await file.save();
  res.send('File renamed successfully');
});

app.delete('/delete/:fileId', authenticate, async (req, res) => {
  try {
    const fileId = new mongoose.Types.ObjectId(req.params.fileId);
    const file = await File.findOne({ _id: fileId, owner: req.user.id });
    if (!file) return res.status(404).send('File not found');
    await File.deleteOne({ _id: fileId });
    gfs.delete(fileId, (err) => {
      if (err) return res.status(500).send('Error deleting file');
      res.send('File deleted successfully');
    });
  } catch (err) {
    res.status(500).send('Internal Server Error');
  }
});

const PORT = 3000;
app.listen(PORT, () => console.log(`Server running on http://localhost:${PORT}`));
