import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';

// https://astro.build/config
export default defineConfig({
	site: 'https://minilab3.ldlework.com',
	integrations: [
		starlight({
			title: 'minilab3',
			social: {
				github: 'https://github.com/dustinlacewell/vcv-minilab3',
			},
			sidebar: [],
			customCss: [
				'./src/styles/custom.scss',
				'./src/styles/content.scss',
			]
		}),
	],
});