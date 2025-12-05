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
    
    async def fetch_page(self, topic):
        """Асинхронно получает HTML страницы"""
        url = f"{self.base_url}/wiki/{topic}"
        try:
            async with self.session.get(url) as response:
                if response.status == 200:
                    html = await response.text()
                    print(f"✓ Successfully fetched: {topic}")
                    return html
                else:
                    print(f"✗ Error {response.status} for: {topic}")
                    return None
        except Exception as e:
            print(f"✗ Error fetching {topic}: {e}")
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
            # Извлекаем ссылки
            links = self.extract_links(html)
            
            # Сортируем и группируем ссылки
            sorted_links = self.analyze_links(links, topic)
            
            # Сохраняем результаты
            await self.save_results(topic, sorted_links, len(links))
            return sorted_links
        
        return None
    
    def analyze_links(self, links, topic):
        """Анализирует и сортирует ссылки"""
        # Удаляем дубликаты по URL
        unique_links = {}
        for link in links:
            if link['url'] not in unique_links:
                unique_links[link['url']] = link
        
        unique_links_list = list(unique_links.values())
        
        # Сортируем по длине текста (предполагая, что более длинные тексты - более значимые ссылки)
        unique_links_list.sort(key=lambda x: x['text_length'], reverse=True)
        
        # Группируем по типам
        links_by_type = {}
        for link in unique_links_list:
            link_type = link['type']
            if link_type not in links_by_type:
                links_by_type[link_type] = []
            links_by_type[link_type].append(link)
        
        return {
            'topic': topic,
            'total_links': len(unique_links_list),
            'links_by_type': links_by_type,
            'all_links': unique_links_list[:100],  # Ограничиваем количество
            'top_links': unique_links_list[:20],   # Топ 20 ссылок
            'timestamp': time.time()
        }
    
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
    
    async def save_to_txt(self, data, filename):
        """Сохраняет в читаемом текстовом формате"""
        content = f"""Wikipedia Links Analysis
Topic: {data['topic']}
Total Unique Links: {data['total_links']}
Timestamp: {time.ctime(data['timestamp'])}

=== TOP 20 LINKS ===
"""
        for i, link in enumerate(data['top_links'], 1):
            content += f"{i:2d}. {link['text']}\n"
            content += f"    URL: {link['url']}\n"
            content += f"    Type: {link['type']}\n"
            content += f"    Text length: {link['text_length']}\n\n"
        
        content += "\n=== LINKS BY TYPE ===\n"
        for link_type, links in data['links_by_type'].items():
            content += f"\n{link_type.upper()} ({len(links)} links):\n"
            for link in links[:10]:  # Показываем первые 10 каждого типа
                content += f"  - {link['text']} -> {link['url']}\n"
        
        async with aiofiles.open(filename, 'w', encoding='utf-8') as f:
            await f.write(content)
    
    async def save_urls_list(self, data, filename):
        """Сохраняет простой список URL"""
        urls = [link['url'] for link in data['all_links']]
        content = '\n'.join(urls)
        async with aiofiles.open(filename, 'w', encoding='utf-8') as f:
            await f.write(content)


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
    
    print("🚀 Starting Wikipedia Links Parser...")
    start_time = time.time()
    
    async with WikipediaLinksParser() as parser:
        # Обрабатываем темы с ограничением одновременных запросов
        tasks = []
        for i, topic in enumerate(topics):
            task = asyncio.create_task(parser.process_topic(topic))
            tasks.append(task)
            
            # Ограничиваем количество одновременных запросов
            if (i) % 4 == 0:
                await asyncio.sleep(3)
        
        results = await asyncio.gather(*tasks)
        
        # Сохраняем общий отчет
        successful_results = [r for r in results if r and not isinstance(r, Exception)]
        await save_global_summary(successful_results)
    
    total_time = time.time() - start_time
    print(f"\n✅ Completed in {total_time:.2f} seconds")
    print(f"📊 Successfully processed: {len(successful_results)}/{len(topics)} topics")

async def save_global_summary(results):
    """Сохраняет глобальную статистику"""
    summary = {
        "total_pages_processed": len(results),
        "processing_time": time.time(),
        "pages": [],
        "global_stats": {
            "total_links_found": 0,
            "average_links_per_page": 0,
            "links_by_type": {}
        }
    }
    
    total_links = 0
    type_counter = {}
    
    for result in results:
        if result:
            summary["pages"].append({
                "topic": result["topic"],
                "links_count": result["total_links"],
                "types_count": {k: len(v) for k, v in result["links_by_type"].items()}
            })
            
            total_links += result["total_links"]
            
            # Считаем типы ссылок
            for link_type, links in result["links_by_type"].items():
                if link_type not in type_counter:
                    type_counter[link_type] = 0
                type_counter[link_type] += len(links)
    
    summary["global_stats"]["total_links_found"] = total_links
    summary["global_stats"]["average_links_per_page"] = total_links / len(results) if results else 0
    summary["global_stats"]["links_by_type"] = type_counter
    
    async with aiofiles.open("links/global_summary.json", 'w', encoding='utf-8') as f:
        await f.write(json.dumps(summary, indent=2, ensure_ascii=False))
    
    print("✓ Global summary saved")

if __name__ == "__main__":
    asyncio.run(main())