# ESP32 Site Generator Prompt

Copy this prompt, answer the questions at the bottom, and paste everything into Claude.

---

## THE PROMPT

```
Create a complete single-page website for ESP32 (LittleFS + ESPAsyncWebServer).

**Stack:** index.html + style.css + app.js — vanilla only, no frameworks.

**Code quality requirements:**
- Code must score 100/100 — clean, semantic, accessible, no unused rules
- CSS uses custom properties (variables) for all colors and fonts
- JS is modular, no inline handlers, passive scroll listeners
- HTML is semantic (header, nav, section, footer) with proper aria labels
- All images use loading="lazy" and descriptive alt text
- Mobile-first responsive, hamburger menu for mobile
- Smooth scroll, scroll-reveal animation on cards and sections
- Contact form with validation (name, email required)
  — POSTs JSON to /api/contact, shows success/error status message

**Reuse these patterns exactly:**
- Fixed header with frosted-glass effect on scroll (.scrolled class)
- Two-span hamburger button toggling .active / .open
- IntersectionObserver for scroll-reveal on cards and section headers
- Horizontal auto-scrolling marquee strip
- Custom cursor dot that expands on hover over interactive elements
- /api/status GET fetch on load → logs ESP32 online status

**Deliver 3 files only:** index.html, style.css, app.js
No main.cpp, no secrets.h — just the frontend files.

---

**Contact form HTML — always use this exact structure (IDs must match the JS):**
```html
<section class="contact" id="contact">
  <div class="container contact-grid">
    <div class="contact-info">
      <span class="section-label">— Contact</span>
      <h2>Get in<br><em>Touch</em></h2>
      <p><!-- contact intro text --></p>
    </div>
    <div class="contact-form-wrap">
      <div id="form-status" class="form-status" aria-live="polite"></div>
      <div class="form-group">
        <label for="fname">Name</label>
        <input type="text" id="fname" placeholder="Your name">
      </div>
      <div class="form-group">
        <label for="femail">Email</label>
        <input type="email" id="femail" placeholder="your@email.com">
      </div>
      <div class="form-group">
        <label for="fcontact">Phone or Telegram</label>
        <input type="text" id="fcontact" placeholder="+380... or @username">
      </div>
      <div class="form-group">
        <label for="fmessage">Message</label>
        <textarea id="fmessage" rows="5" placeholder="Your message..."></textarea>
      </div>
      <button id="submitBtn" class="btn-submit">
        <span id="btnText">Send Message</span>
        <span class="btn-arrow">→</span>
      </button>
    </div>
  </div>
</section>
```

**Contact form JS — always use this exact logic (POSTs to ESP32, ESP32 forwards to Telegram):**
```js
// Contact form → /api/contact
// The ESP32 holds the Telegram token — it never appears in this file.
const submitBtn = document.getElementById('submitBtn');
const btnText   = document.getElementById('btnText');
const formStatus = document.getElementById('form-status');

function setStatus(msg, cls) {
  formStatus.textContent = msg;
  formStatus.className = 'form-status ' + cls;
}

submitBtn.addEventListener('click', async () => {
  const name    = document.getElementById('fname').value.trim();
  const email   = document.getElementById('femail').value.trim();
  const contact = document.getElementById('fcontact').value.trim();
  const message = document.getElementById('fmessage').value.trim();

  if (!name || !email || !message) {
    setStatus('Please fill in all required fields.', 'error');
    return;
  }
  if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email)) {
    setStatus('Please enter a valid email address.', 'error');
    return;
  }

  submitBtn.disabled  = true;
  btnText.textContent = 'Sending...';
  setStatus('Sending your message...', 'sending');

  try {
    const res  = await fetch('/api/contact', {
      method:  'POST',
      headers: { 'Content-Type': 'application/json' },
      body:    JSON.stringify({ name, email, contact, message })
    });
    const data = await res.json();

    if (data.ok) {
      setStatus("Message sent. We'll be in touch soon.", 'success');
      ['fname','femail','fcontact','fmessage'].forEach(id => {
        document.getElementById(id).value = '';
      });
    } else {
      setStatus('Failed to send. Please try again.', 'error');
    }
  } catch {
    setStatus('Network error. Please try again.', 'error');
  } finally {
    submitBtn.disabled  = false;
    btnText.textContent = 'Send Message';
  }
});
```
```

---

## FILL IN YOUR DETAILS  /  ЗАПОВНИ СВОЇ ДАНІ

**Business / Бізнес**
- Name / Назва: 
- Type / Тип: (e.g. ceramics studio / photography / woodworking / bakery)
  (напр. керамічна студія / фотографія / столярна майстерня / пекарня)
- Tagline / Слоган: (hero headline, 4–8 words / заголовок героя, 4–8 слів)
- About text / Про нас: (2–3 sentences / 2–3 речення)
- Language / Мова: 

**Design / Дизайн**
- Color scheme / Кольорова схема: (e.g. "dark background, gold accent" / напр. "темний фон, золотий акцент")
- Font style / Стиль шрифту: (e.g. "serif + sans-serif elegant" / напр. "елегантний serif + sans-serif")

**Sections & content / Секції та контент**
- Sections / Секції: (choose / обери: Hero / About / Products / Portfolio / Services / Contact)
- Cards / Картки: (products or portfolio — name + price or description / назва + ціна або опис)
- Marquee words / Слова бігучого рядка: (4–6 keywords separated by · / ключові слова через ·)
- Contact intro / Вступ до контактів: (1 sentence / 1 речення)

**Images / Зображення**
- [ ] I will upload my own — list filenames / Завантажу свої — вкажи назви файлів: (напр. hero.webp, item1.webp ...)
- [ ] Use Unsplash — describe each / Використай Unsplash — опиши кожне: (напр. "кошик з лози на столі")
