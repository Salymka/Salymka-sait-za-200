// ── CONFIG ──────────────────────────────────────────────
const TELEGRAM_BOT_TOKEN = '8837848065:AAGAlja-DFSQVYwj6nXK7RceYb_cEQXFI4M';
const TELEGRAM_CHAT_ID   = '525534862';
// ────────────────────────────────────────────────────────

// Custom cursor
const cursor = document.querySelector('.cursor');
if (cursor) {
  document.addEventListener('mousemove', e => {
    cursor.style.left = e.clientX + 'px';
    cursor.style.top  = e.clientY + 'px';
  });

  document.querySelectorAll('a, button, .card, input, textarea').forEach(el => {
    el.addEventListener('mouseenter', () => cursor.classList.add('expand'));
    el.addEventListener('mouseleave', () => cursor.classList.remove('expand'));
  });
}

// Header scroll effect
const header = document.getElementById('header');
window.addEventListener('scroll', () => {
  header.classList.toggle('scrolled', window.scrollY > 60);
}, { passive: true });

// Mobile menu
const menuBtn = document.getElementById('menuBtn');
const menu    = document.getElementById('menu');

menuBtn.addEventListener('click', () => {
  menu.classList.toggle('active');
  menuBtn.classList.toggle('open');
});

menu.querySelectorAll('a').forEach(link => {
  link.addEventListener('click', () => {
    menu.classList.remove('active');
    menuBtn.classList.remove('open');
  });
});

// Scroll reveal
const revealEls = document.querySelectorAll(
  '.card, .about-text, .contact-info, .contact-form-wrap, .section-header'
);

revealEls.forEach(el => el.classList.add('reveal'));

const observer = new IntersectionObserver(entries => {
  entries.forEach((entry, i) => {
    if (entry.isIntersecting) {
      setTimeout(() => entry.target.classList.add('visible'), i * 80);
      observer.unobserve(entry.target);
    }
  });
}, { threshold: 0.1 });

revealEls.forEach(el => observer.observe(el));

// Status API ping
fetch('/api/status')
  .then(r => r.json())
  .then(d => console.log('ESP32:', d.status))
  .catch(() => {});

// Contact form -> Telegram
const submitBtn = document.getElementById('submitBtn');
const btnText   = document.getElementById('btnText');
const status    = document.getElementById('form-status');

function setStatus(msg, cls) {
  status.textContent = msg;
  status.className = 'form-status ' + cls;
}

submitBtn.addEventListener('click', async () => {
  const name    = document.getElementById('fname').value.trim();
  const email   = document.getElementById('femail').value.trim();
  const contact = document.getElementById('fcontact').value.trim();
  const message = document.getElementById('fmessage').value.trim();

  if (!name || !email || !message) {
    setStatus('Please fill in all fields.', 'error');
    return;
  }

  if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email)) {
    setStatus('Please enter a valid email address.', 'error');
    return;
  }

  submitBtn.disabled = true;
  btnText.textContent = 'Sending...';
  setStatus('Sending your message...', 'sending');

  const text =
    '*New message — Handwoven*\n\n' +
    '*Name:* ' + name + '\n' +
    '*Email:* ' + email + '\n' +
    (contact ? '*Phone / Telegram:* ' + contact + '\n' : '') +
    '\n*Message:*\n' + message;

  try {
    const res = await fetch(
      'https://api.telegram.org/bot' + TELEGRAM_BOT_TOKEN + '/sendMessage',
      {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          chat_id:    TELEGRAM_CHAT_ID,
          text:       text,
          parse_mode: 'Markdown'
        })
      }
    );

    const data = await res.json();

    if (data.ok) {
      setStatus("Message sent. We'll be in touch soon.", 'success');
      document.getElementById('fname').value    = '';
      document.getElementById('femail').value   = '';
      document.getElementById('fcontact').value = '';
      document.getElementById('fmessage').value = '';
    } else {
      setStatus('Failed to send. Please try again.', 'error');
    }
  } catch (err) {
    setStatus('Network error. Please try again.', 'error');
  } finally {
    submitBtn.disabled  = false;
    btnText.textContent = 'Send Message';
  }
});