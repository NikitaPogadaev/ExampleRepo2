import asyncio
import aiohttp
import aiofiles
from bs4 import BeautifulSoup
import json
import time
import random
import re
from urllib.parse import urljoin, urlparse

class WikipediaLinksParser:
    def __init__(self):
        self.base_url = "https://en.wikipedia.org"
        self.session = None
        self.user_agents = [
            'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
            'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
            'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
            'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:89.0) Gecko/20100101 Firefox/89.0',
        ]
    
    def get_headers(self):
        return {
            'User-Agent': random.choice(self.user_agents),
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate, br',
            'DNT': '1',
            'Connection': 'keep-alive',
        }
    
    async def __aenter__(self):
        timeout = aiohttp.ClientTimeout(total=30)
        connector = aiohttp.TCPConnector(limit=5, verify_ssl=False)
        self.session = aiohttp.ClientSession(
            timeout=timeout,
            connector=connector,
            headers=self.get_headers()
        )
        return self
    
    async def __aexit__(self, exc_type, exc_val, exc_tb):
        await self.session.close()
    
    async def fetch_page(self, url):
        """Асинхронно получает HTML страницы"""
        try:
            async with self.session.get(url) as response:
                if response.status == 200:
                    html = await response.text()
                    print(f"✓ Successfully fetched: {url}")
                    return html
                else:
                    print(f"✗ Error {response.status} for: {url}")
                    return None
        except Exception as e:
            print(f"✗ Error fetching {url}: {e}")
            return None
    
    def extract_links(self, html):
        """Извлекает все ссылки со страницы"""
        if not html:
            return []
        
        soup = BeautifulSoup(html, 'html.parser')
        links_data = []
        
        # Основной контент страницы
        content_div = soup.find('div', {'id': 'mw-content-text'})
        if not content_div:
            return []
        
        all_links = content_div.find_all('a', href=True)
        
        for link in all_links:
            href = link.get('href', '')
            text = link.get_text(strip=True)
            
            # Фильтруем только внутренние ссылки Wikipedia
            if self.is_valid_wikipedia_link(href):
                full_url = urljoin(self.base_url, href)
                link_type = self.classify_link(href, text)
                
                links_data.append({
                    'text': text,
                    'url': full_url,
                    'href': href,
                    'type': link_type,
                    'text_length': len(text)
                })
        
        return links_data
    
    def is_valid_wikipedia_link(self, href):
        """Проверяет, является ли ссылка валидной внутренней ссылкой Wikipedia"""
        # Исключаем внешние ссылки, файлы, служебные страницы
        excluded_patterns = [
            r'^https?://',  # внешние ссылки
            r'^#',          # якоря
            r'^/wiki/File:', # файлы
            r'^/wiki/Category:', # категории
            r'^/wiki/Special:', # специальные страницы
            r'^/wiki/Help:', # справка
            r'^/wiki/Template:', # шаблоны
            r'^/wiki/Portal:', # порталы
            r'^/wiki/Wikipedia:', # страницы википедии
            r'^/wiki/Talk:', # обсуждения
            r'^/wiki/User:', # пользователи
        ]

        if href.startswith('/wiki/') and not any(re.match(pattern, href) for pattern in excluded_patterns):
            return True
        return False
    
    def classify_link(self, href, text):
        """Классифицирует тип ссылки"""
        if not text or len(text.strip()) == 0:
            return 'empty_text'
        elif len(text) > 50:
            return 'long_text'
        elif href.startswith('/wiki/Portal:'):
            return 'portal'
        elif 'disambiguation' in href.lower():
            return 'disambiguation'
        elif 'list_of' in href.lower():
            return 'list'
        else:
            return 'article'
    
    async def process_topic(self, topic):
        """Полный процесс обработки одной темы"""
        print(f"🔄 Processing: {topic}")
        
        # Задержка между запросами
        await asyncio.sleep(random.uniform(1, 2))
        
        # Получаем HTML
        html = await self.fetch_page(topic)
        
        if html:
            await self.save_page(html)
        
        return None
    
    async def save_page(self, data):
        """Сохраняет результаты в файлы"""
        
        # Сохраняем в JSON
        await self.save_to_json(data, "./page_dump.json")
    
    async def save_results(self, topic, data, total_found):
        """Сохраняет результаты в файлы"""
        safe_topic = "".join(c for c in topic if c.isalnum() or c in ('_', '-')).rstrip()
        
        # Сохраняем в JSON
        await self.save_to_json(data, f"links/json/{safe_topic}_links.json")
        
        # Сохраняем в TXT
        await self.save_to_txt(data, f"links/txt/{safe_topic}_links.txt")
        
        # Сохраняем простой список URL
        await self.save_urls_list(data, f"links/urls/{safe_topic}_urls.txt")
        
        print(f"✓ Saved {total_found} links from: {topic}")
    
    async def save_to_json(self, data, filename):
        """Сохраняет в JSON формате"""
        async with aiofiles.open(filename, 'w', encoding='utf-8') as f:
            await f.write(json.dumps(data, indent=2, ensure_ascii=False))



def load_topics_from_config(config_file="topics_config.txt"):
    with open(config_file, 'r', encoding='utf-8') as f:
        content = f.read()
    
    topics = []
    for line in content.split('\n'):
        line = line.strip()
        if line and not line.startswith('#'):
            topics.append(line)
    return topics

async def main():

    topics = load_topics_from_config()
    
    # Создаем директории
    import os
    os.makedirs("links/json", exist_ok=True)
    os.makedirs("links/txt", exist_ok=True)
    os.makedirs("links/urls", exist_ok=True)
    
    start_time = time.time()
    
    async with WikipediaLinksParser() as parser:
        tasks = []
        for i, topic in enumerate(topics):
            task = asyncio.create_task(parser.process_topic(topic))
            tasks.append(task)
            if (i) % 4 == 0:
                await asyncio.sleep(3)
        
        results = await asyncio.gather(*tasks)
        
        # Сохраняем общий отчет
        successful_results = [r for r in results if r and not isinstance(r, Exception)]
    
    total_time = time.time() - start_time
    print(f"\n✅ Completed in {total_time:.2f} seconds")
    print(f"📊 Successfully processed: {len(successful_results)}/{len(topics)} topics")


if __name__ == "__main__":
    asyncio.run(main())